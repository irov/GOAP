/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TaskSemaphore.h"

#include "GOAP/NodeInterface.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskSemaphore::TaskSemaphore( Allocator * _allocator, const SemaphoreInterfacePtr & _semaphore, uint32_t _flags, int32_t _test, int32_t _apply )
        : TaskInterface( _allocator )
        , m_semaphore( _semaphore )
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
        int32_t value = m_semaphore->getValue();

        if( m_flags & FLAG_SEMAPHORE_TEST_EQUAL )
        {
            if( m_test != value )
            {
                return true;
            }
        }
        else if( m_flags & FLAG_SEMAPHORE_TEST_NOTEQUAL )
        {
            if( m_test == value )
            {
                return true;
            }
        }
        else if( m_flags & FLAG_SEMAPHORE_TEST_GREATER )
        {
            if( m_test >= value )
            {
                return true;
            }
        }
        else if( m_flags & FLAG_SEMAPHORE_TEST_LESS )
        {
            if( m_test <= value )
            {
                return true;
            }
        }
        else if( m_flags & FLAG_SEMAPHORE_TEST_GREATEREQUAL )
        {
            if( m_test > value )
            {
                return true;
            }
        }
        else if( m_flags & FLAG_SEMAPHORE_TEST_LESSEQUAL )
        {
            if( m_test < value )
            {
                return true;
            }
        }

        this->process();

        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskSemaphore::_onRun( NodeInterface * _node )
    {
        Allocator * allocator = this->getAllocator();

        m_provider = m_semaphore->addProvider( allocator, [this, _node]()
        {
            bool result = this->test( _node );

            return result;
        } );

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
        if( m_provider != nullptr )
        {
            m_semaphore->removeObserverProvider( m_provider );
            m_provider = nullptr;
        }

        m_semaphore = nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskSemaphore::_onSkipable() const
    {
        if( m_flags & FLAG_SEMAPHORE_MASK_TEST )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskSemaphore::test( NodeInterface * _node )
    {
        int32_t value = m_semaphore->getValue();

        if( m_flags & FLAG_SEMAPHORE_TEST_EQUAL )
        {
            if( m_test != value )
            {
                return false;
            }
        }
        else if( m_flags & FLAG_SEMAPHORE_TEST_NOTEQUAL )
        {
            if( m_test == value )
            {
                return false;
            }
        }
        else if( m_flags & FLAG_SEMAPHORE_TEST_LESS )
        {
            if( m_test <= value )
            {
                return false;
            }
        }
        else if( m_flags & FLAG_SEMAPHORE_TEST_GREATER )
        {
            if( m_test >= value )
            {
                return false;
            }
        }
        else if( m_flags & FLAG_SEMAPHORE_TEST_LESSEQUAL )
        {
            if( m_test < value )
            {
                return false;
            }
        }
        else if( m_flags & FLAG_SEMAPHORE_TEST_GREATEREQUAL )
        {
            if( m_test > value )
            {
                return false;
            }
        }
        else
        {
            return false;
        }

        if( m_provider != nullptr )
        {
            m_semaphore->removeObserverProvider( m_provider );
            m_provider = nullptr;
        }

        if( _node->isSkip() == false )
        {
            _node->complete();
        }
        else
        {
            _node->skip();
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskSemaphore::process()
    {
        if( m_flags & FLAG_SEMAPHORE_APPLY_ASSIGN )
        {
            m_semaphore->setValue( m_apply );
        }
        else if( m_flags & FLAG_SEMAPHORE_APPLY_ADD )
        {
            m_semaphore->addValue( m_apply );
        }
        else if( m_flags & FLAG_SEMAPHORE_APPLY_SUBTRACT )
        {
            m_semaphore->subtractValue( m_apply );
        }
    }
}