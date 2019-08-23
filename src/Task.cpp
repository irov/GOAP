/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/Task.h"
#include "GOAP/Source.h"
#include "GOAP/Chain.h"

#include <algorithm>

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    Task::Task()
        : m_state( TASK_STATE_IDLE )
        , m_skip( false )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Task::~Task()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::setChain( const ChainPtr & _chain )
    {
        m_chain = _chain;
    }
    //////////////////////////////////////////////////////////////////////////
    const ChainPtr & Task::getChain() const
    {
        return m_chain;
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::throwError()
    {
        m_state = TASK_STATE_ERROR;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::isIdle() const
    {
        return m_state == TASK_STATE_IDLE;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::isRunning() const
    {
        return m_state == TASK_STATE_RUN;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::isEnd() const
    {
        return m_state == TASK_STATE_END;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::isSkip() const
    {
        return m_skip;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::isError() const
    {
        return m_state == TASK_STATE_END;
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::addNext( const TaskPtr & _task )
    {
        m_nexts.push_back( _task );
        _task->addPrev_( TaskPtr::from( this ) );
    }
    //////////////////////////////////////////////////////////////////////////
    VectorTasks & Task::getNexts()
    {
        return m_nexts;
    }
    //////////////////////////////////////////////////////////////////////////
    const VectorTasks & Task::getNexts() const
    {
        return m_nexts;
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::popNexts( VectorTasks & _clone )
    {
        for( const TaskPtr & next : m_nexts )
        {
            next->removePrev_( TaskPtr::from( this ) );
        }

        _clone.swap( m_nexts );
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::injectSource( const SourcePtr & _source )
    {
        if( _source->empty() == true )
        {
            return true;
        }

        VectorTasks nexts;
        this->popNexts( nexts );

        const ChainPtr & chain = this->getChain();

        TaskPtr task = _source->parse( chain, TaskPtr::from( this ) );

        if( task == nullptr )
        {
            return false;
        }

        for( const TaskPtr & next : nexts )
        {
            task->addNext( next );
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::forkSource( const SourcePtr & _source )
    {
        if( _source->empty() == true )
        {
            return true;
        }

        const ChainPtr & chain = this->getChain();

        TaskPtr task = _source->parse( chain, TaskPtr::from( this ) );

        if( task == nullptr )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::run( bool _checkSkipedFalse )
    {
        if( m_state != TASK_STATE_IDLE )
        {
            return false;
        }

        if( this->onValidate() == false )
        {
            return false;
        }

        this->setState( TASK_STATE_RUN );

        m_chain->runTask( TaskPtr::from( this ) );

        if( this->onInitialize() == false )
        {
            return false;
        }

        if( this->onCheck() == false )
        {
            this->complete( false );

            return true;
        }

        bool done = this->onRun();

        if( m_state != TASK_STATE_RUN )
        {
            return true;
        }

        if( done == false )
        {
            return true;
        }

        if( _checkSkipedFalse == true )
        {
            if( this->onSkipBlock() == true )
            {
                this->complete( true, false );

                return true;
            }

            if( this->onSkipable() == false )
            {
                return false;
            }
        }

        this->complete( true, false );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::skip()
    {
        if( m_state == TASK_STATE_SKIP )
        {
            return true;
        }

        if( m_state == TASK_STATE_END )
        {
            return true;
        }

        if( m_skip == true )
        {
            return false;
        }

        if( this->onSkipable() == false )
        {
            if( m_state == TASK_STATE_IDLE )
            {
                if( this->run( true ) == false )
                {
                    return false;
                }
            }
            else
            {
                this->onSkipNoSkiped();
            }

            return true;
        }

        m_skip = true;

        switch( m_state )
        {
        case TASK_STATE_IDLE:
            {
                if( this->onInitialize() == false )
                {
                    return false;
                }

                m_chain->runTask( TaskPtr::from( this ) );

                if( this->onFastSkip() == false )
                {
                    this->setState( TASK_STATE_RUN );

                    if( this->onCheck() == true )
                    {
                        bool done = this->onRun();

                        if( m_state == TASK_STATE_END )
                        {
                            return true;
                        }

                        this->setState( TASK_STATE_SKIP );

                        if( done == false )
                        {
                            this->onSkip();
                        }
                    }

                    this->onFinally();
                }

                this->taskSkip_();
            }break;
        case TASK_STATE_RUN:
            {
                this->setState( TASK_STATE_SKIP );

                this->onSkip();
                this->onFinally();

                this->taskSkip_();
            }break;
        default:
            break;
        }

        if( m_state != TASK_STATE_END )
        {
            this->finalize_();
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::cancel( bool _withNexts )
    {
        if( m_state == TASK_STATE_END )
        {
            return;
        }

        m_skip = true;

        if( m_state == TASK_STATE_RUN )
        {
            this->setState( TASK_STATE_CANCEL );

            this->onSkip();
            this->onCancel();
            this->onFinally();

            m_chain->completeTask( TaskPtr::from( this ) );
        }

        if( _withNexts == true )
        {
            VectorTasks copy_nexts = m_nexts;

            for( const TaskPtr & task : copy_nexts )
            {
                task->cancel( _withNexts );
            }
        }

        this->finalize_();
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::taskSkip_()
    {
        if( m_state == TASK_STATE_END )
        {
            return;
        }

        m_chain->completeTask( TaskPtr::from( this ) );

        VectorTasks copy_nexts = std::move( m_nexts );

        for( const TaskPtr & next : copy_nexts )
        {
            if( next->prevSkip_( TaskPtr::from( this ) ) == true )
            {
                m_chain->processTask( next, true );
            }
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::complete( bool _running, bool _skiped )
    {
        switch( m_state )
        {
        case TASK_STATE_SKIP:
        case TASK_STATE_END:
        case TASK_STATE_CANCEL:
            return;
        default:
            break;
        };

        if( m_state != TASK_STATE_RUN )
        {
            return;
        }

        if( _skiped == true )
        {
            m_skip = true;
            this->setState( TASK_STATE_SKIP );
        }
        else
        {
            this->setState( TASK_STATE_COMPLETE );
        }

        if( _running == true )
        {
            this->onComplete();
            this->onFinally();
        }

        this->setState( TASK_STATE_END );

        if( m_skip == false )
        {
            VectorTasks copy_nexts = std::move( m_nexts );

            for( const TaskPtr & next : copy_nexts )
            {
                if( next->prevComplete_( TaskPtr::from( this ) ) == true )
                {
                    m_chain->processTask( next, false );
                }
            }
        }
        else
        {
            VectorTasks copy_nexts = std::move( m_nexts );

            for( const TaskPtr & next : copy_nexts )
            {
                if( next->prevSkip_( TaskPtr::from( this ) ) == true )
                {
                    m_chain->processTask( next, true );
                }
            }
        }

        ChainPtr chain = m_chain;

        this->finalize_();

        chain->completeTask( TaskPtr::from( this ) );
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::prevSkip_( const TaskPtr & _task )
    {
        if( m_state == TASK_STATE_END )
        {
            return false;
        }

#	ifdef _DEBUG
        if( this->hasPrev_( _task ) == false )
        {
            return false;
        }
#	endif

        this->unlink_( _task );

        if( this->onCheckSkip() == false )
        {
            return false;
        }

        this->cancelPrev_();

        if( m_state == TASK_STATE_END )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::skipPrev_()
    {
        VectorTasks clone_prevs = m_prevs;

        for( const TaskPtr & prev : clone_prevs )
        {
            switch( prev->m_state )
            {
            case TASK_STATE_IDLE:
                {
                    prev->skipPrev_();
                    prev->cancelPrev_();
                }break;
            case  TASK_STATE_RUN:
                {
                    prev->skip();
                    prev->cancel();
                }break;
            default:
                break;
            }
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::cancelPrev_()
    {
        VectorTasks clone_prevs = m_prevs;

        for( const TaskPtr & prev : clone_prevs )
        {
            switch( prev->m_state )
            {
            case TASK_STATE_IDLE:
                {
                    prev->cancelPrev_();
                    prev->cancel();
                    this->unlink_( prev );
                }break;
            case  TASK_STATE_RUN:
                {
                    prev->cancel();
                    this->unlink_( prev );
                }break;
            default:
                break;
            }
        }
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::prevComplete_( const TaskPtr & _task )
    {
        if( m_state != TASK_STATE_IDLE )
        {
            return false;
        }

#	ifdef _DEBUG
        if( this->hasPrev_( _task ) == false )
        {
            return false;
        }
#	endif

        this->unlink_( _task );

        if( this->onCheckRun() == false )
        {
            return false;
        }

        this->cancelPrev_();

        if( m_state == TASK_STATE_END )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::onInitialize()
    {
        bool successful = this->_onInitialize();

        return successful;
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::onFinalize()
    {
        this->_onFinalize();
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::onValidate() const
    {
        bool valid = this->_onValidate();

        return valid;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::onCheck()
    {
        bool check = this->_onCheck();

        return check;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::onRun()
    {
        bool successful = this->_onRun();

        return successful;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::onSkipable() const
    {
        bool result = this->_onSkipable();

        return result;
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::onSkipNoSkiped()
    {
        this->_onSkipNoSkiped();
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::onSkipBlock()
    {
        bool result = this->_onSkipBlock();

        return result;
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::onComplete()
    {
        this->_onComplete();
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::onFastSkip()
    {
        bool result = this->_onFastSkip();

        return result;
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::onSkip()
    {
        this->_onSkip();
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::onCancel()
    {
        this->_onCancel();
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::onFinally()
    {
        this->_onFinally();
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::onCheckRun() const
    {
        bool result = this->_onCheckRun();

        return result;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::onCheckSkip() const
    {
        bool result = this->_onCheckSkip();

        return result;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::_onInitialize()
    {
        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::_onFinalize()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::_onValidate() const
    {
        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::_onCheck()
    {
        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::_onRun()
    {
        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::_onSkipable() const
    {
        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::_onSkipNoSkiped()
    {
        //Empty
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::_onSkipBlock()
    {
        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::_onComplete()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::_onFastSkip()
    {
        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::_onSkip()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::_onCancel()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::_onFinally()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::_onCheckRun() const
    {
        bool result = m_prevs.empty();

        return result;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::_onCheckSkip() const
    {
        bool result = m_prevs.empty();

        return result;
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::addPrev_( const TaskPtr & _task )
    {
        m_prevs.push_back( _task );
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::removePrev_( const TaskPtr & _task )
    {
        VectorTasks::iterator it_erase = std::find( m_prevs.begin(), m_prevs.end(), _task );

        m_prevs.erase( it_erase );
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::removeNext_( const TaskPtr & _task )
    {
        VectorTasks::iterator it_erase = std::find( m_nexts.begin(), m_nexts.end(), _task );

        if( it_erase == m_nexts.end() )
        {
            return;
        }

        m_nexts.erase( it_erase );
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::unlink_( const TaskPtr & _task )
    {
        this->removePrev_( _task );
        _task->removeNext_( TaskPtr::from( this ) );
    }
    //////////////////////////////////////////////////////////////////////////
    bool Task::hasPrev_( const TaskPtr & _task ) const
    {
        VectorTasks::const_iterator it_found = std::find( m_prevs.begin(), m_prevs.end(), _task );

        return it_found != m_prevs.end();
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::finalize_()
    {
        this->setState( TASK_STATE_END );

        this->onFinalize();

        m_chain = nullptr;

        m_nexts.clear();
        m_prevs.clear();
    }
    //////////////////////////////////////////////////////////////////////////
    void Task::setState( ETaskState _state )
    {
        m_state = _state;
    }
}