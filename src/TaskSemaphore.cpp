/*
* Copyright (C) 2017, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	include "GOAP/TaskSemaphore.h"
#   include "GOAP/EventProvider.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class TaskSemaphore::EventProviderTaskSemaphore
        : public EventProvider
    {
    public:
        EventProviderTaskSemaphore( TaskSemaphore * _task )
            : m_task( _task )
        { }

    protected:
        bool onEvent() override
        {
            bool result = m_task->test();

            return result;
        }

    protected:
        TaskSemaphore * m_task;
    };
    //////////////////////////////////////////////////////////////////////////
    TaskSemaphore::TaskSemaphore( const SemaphorePtr & _semaphore, uint32_t _flags, int32_t _test, int32_t _apply )
        : m_semaphore( _semaphore )
        , m_flags( _flags )
        , m_test( _test )
        , m_apply( _apply )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskSemaphore::~TaskSemaphore()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskSemaphore::_onCheck()
    {
        if( m_flags & TASK_SEMAPHORE_FROM )
        {
            if( m_semaphore->equalValue( m_test ) == false )
            {
                return true;
            }
        }
        else if( m_flags & TASK_SEMAPHORE_LESS )
        {
            if( m_semaphore->lessValue( m_test ) == false )
            {
                return true;
            }
        }

        this->process();

        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskSemaphore::_onRun()
    {
        m_observer = m_semaphore->addObserver( GOAP_NEW EventProviderTaskSemaphore( this ) );
        
        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskSemaphore::_onComplete()
    {
        this->process();
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskSemaphore::_onFinally()
    {
        m_semaphore->removeObserver( m_observer );

        m_semaphore = nullptr;
        m_observer = nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskSemaphore::test()
    {
        if( m_flags & TASK_SEMAPHORE_FROM )
        {
            if( m_semaphore->equalValue( m_test ) == false )
            {
                return false;
            }
        }
        else if( m_flags & TASK_SEMAPHORE_LESS )
        {
            if( m_semaphore->lessValue( m_test ) == false )
            {
                return false;
            }
        }
        else
        {
            return false;
        }

        m_observer = nullptr;

        if( this->isSkip() == false )
        {
            this->complete();
        }
        else
        {
            this->skip();
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskSemaphore::process()
    {
        if( m_flags & TASK_SEMAPHORE_TO )
        {
            m_semaphore->setValue( m_apply );
        }
        else if( m_flags & TASK_SEMAPHORE_ADD )
        {
            m_semaphore->addValue( m_apply );
        }
        else if( m_flags & TASK_SEMAPHORE_SUBTRACT )
        {
            m_semaphore->subtractValue( m_apply );
        }
    }
}