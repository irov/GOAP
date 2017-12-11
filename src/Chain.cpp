/*
* Copyright (C) 2017, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	include "GOAP/Chain.h"
#	include "GOAP/Task.h"
#	include "GOAP/Source.h"

#	include "GOAP/ChainProvider.h"

#	include "GOAP/TaskDummy.h"

#	include <algorithm>

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class Chain::TaskChainEnd
        : public Task
    {
    public:
        explicit TaskChainEnd( Chain * _chain )
            : m_chain( _chain )
        {
        }

    public:
        bool _onRun()
        {
            bool skip = this->isSkip();

            m_chain->complete( skip );

            return true;
        }
        
    protected:
        Chain * m_chain;
    };
    //////////////////////////////////////////////////////////////////////////
    Chain::Chain( const SourcePtr & _source )
        : m_source( _source )
        , m_state( TASK_CHAIN_STATE_IDLE )
        , m_complete( false )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Chain::~Chain()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::setCallbackProvider( const ChainProviderPtr & _cb )
    {
        m_cb = _cb;
    }
    //////////////////////////////////////////////////////////////////////////
    const ChainProviderPtr & Chain::getCallbackProvider() const
    {
        return m_cb;
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::addFork( const ChainPtr & _fork )
    {
        m_forks.push_back( _fork );
    }
    //////////////////////////////////////////////////////////////////////////
    bool Chain::removeFork( const ChainPtr & _fork )
    {
        VectorChains::iterator it_found = std::find( m_forks.begin(), m_forks.end(), _fork );

        if( it_found == m_forks.end() )
        {
            return false;
        }

        m_forks.erase( it_found );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Chain::run()
    {
        ETaskChainState state = this->getState_();

        if( state != TASK_CHAIN_STATE_IDLE )
        {
            return false;
        }

        IntrusiveThisAcquire( this );

        this->setState_( TASK_CHAIN_STATE_RUN );

        TaskPtr task_first = GOAP_NEW TaskDummy();
        task_first->setChain( this );

        TaskPtr task_last = m_source->parse( this, task_first );
        
        TaskPtr task_cb = GOAP_NEW TaskChainEnd( this );
        task_cb->setChain( this );

        task_last->addNext( task_cb );

        bool skip = m_source->isSkip();

        this->processTask( task_first, skip );

        IntrusiveThisRelease( this );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::cancel()
    {
        IntrusiveThisAcquire( this );

        VectorChains copy_forks = m_forks;

        for( VectorChains::const_iterator
            it = copy_forks.begin(),
            it_end = copy_forks.end();
            it != it_end;
            ++it )
        {
            const ChainPtr & fork = *it;

            fork->cancel();
        }

        m_forks.clear();

        if( m_state != TASK_CHAIN_STATE_IDLE &&
            m_state != TASK_CHAIN_STATE_CANCEL &&
            m_state != TASK_CHAIN_STATE_FINALIZE )
        {
            if( m_state == TASK_CHAIN_STATE_RUN )
            {
                this->skipRunningTasks_();
                this->cancelRunningTasks_();
            }

            //if( m_state != TASK_CHAIN_STATE_COMPLETE &&
            //	m_state != TASK_CHAIN_STATE_CANCEL &&
            //	m_state != TASK_CHAIN_STATE_FINALIZE )
            //{

            //}
        }

        if( m_state != TASK_CHAIN_STATE_CANCEL &&
            m_state != TASK_CHAIN_STATE_FINALIZE )
        {
            this->setState_( TASK_CHAIN_STATE_CANCEL );

            this->finalize_();
        }

        IntrusiveThisRelease( this );
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::skip()
    {
        IntrusiveThisAcquire( this );

        VectorChains copy_forks = m_forks;

        for( VectorChains::const_iterator
            it = copy_forks.begin(),
            it_end = copy_forks.end();
            it != it_end;
            ++it )
        {
            const ChainPtr & fork = *it;

            fork->skip();
        }

        if( m_state == TASK_CHAIN_STATE_RUN )
        {
            this->skipRunningTasks_();
        }

        IntrusiveThisRelease( this );
    }
    //////////////////////////////////////////////////////////////////////////
    bool Chain::isComplete() const
    {
        return m_complete;
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::runTask( const TaskPtr & _task )
    {
        m_runningTasks.push_back( _task );
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::completeTask( const TaskPtr & _task )
    {
        VectorTasks::iterator it_found = std::find( m_runningTasks.begin(), m_runningTasks.end(), _task );

        if( it_found == m_runningTasks.end() )
        {
            return;
        }

        m_runningTasks.erase( it_found );
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::processTask( const TaskPtr & _task, bool _skip )
    {
        if( _skip == true )
        {
            _task->skip();
        }
        else
        {
            _task->run( false );
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::complete( bool _skip )
    {
        m_complete = true;
        this->setState_( TASK_CHAIN_STATE_COMPLETE );

        ChainProviderPtr cb = m_cb;

        this->finalize_();

        if( cb != nullptr )
        {
            cb->onChain( _skip );
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::skipRunningTasks_()
    {
        VectorTasks tasks = m_runningTasks;

        for( VectorTasks::const_iterator
            it = tasks.begin(),
            it_end = tasks.end();
            it != it_end;
            ++it )
        {
            const TaskPtr & task = *it;

            task->skip();
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::cancelRunningTasks_()
    {
        VectorTasks tasks = m_runningTasks;

        for( VectorTasks::const_iterator
            it = tasks.begin(),
            it_end = tasks.end();
            it != it_end;
            ++it )
        {
            const TaskPtr & task = *it;

            task->cancel( true );
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::finalize_()
    {
        VectorChains copy_forks = m_forks;

        for( VectorChains::const_iterator
            it = copy_forks.begin(),
            it_end = copy_forks.end();
            it != it_end;
            ++it )
        {
            const ChainPtr & fork = *it;

            fork->cancel();
        }

        m_forks.clear();

        m_source = nullptr;
        m_cb = nullptr;
        //m_runningTasks.clear();

        this->setState_( TASK_CHAIN_STATE_FINALIZE );
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::setState_( ETaskChainState _state )
    {
        m_state = _state;
    }
    //////////////////////////////////////////////////////////////////////////
    Chain::ETaskChainState Chain::getState_() const
    {
        return m_state;
    }
}