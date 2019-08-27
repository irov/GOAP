/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/Chain.h"
#include "GOAP/Task.h"
#include "GOAP/Source.h"

#include "GOAP/ChainProvider.h"

#include "GOAP/TaskDummy.h"

#include <algorithm>

namespace GOAP
{
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
    bool Chain::run()
    {
        ETaskChainState state = this->getState_();

        if( state != TASK_CHAIN_STATE_IDLE )
        {
            return false;
        }

        this->incref();

        this->setState_( TASK_CHAIN_STATE_RUN );

        m_source->addFunctionContext( [this]( bool _isSkip )
        {
            this->complete( _isSkip );
        } );

        TaskPtr task_first( new TaskDummy() );
        task_first->setChain( ChainPtr::from( this ) );

        m_source->parse( ChainPtr::from( this ), task_first );

        bool skip = m_source->isSkip();

        this->processTask( task_first, skip );

        this->decref();

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::cancel()
    {
        this->incref();

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

        ChainProviderPtr cb = std::move( m_cb );

        if( m_state != TASK_CHAIN_STATE_CANCEL &&
            m_state != TASK_CHAIN_STATE_FINALIZE )
        {
            this->setState_( TASK_CHAIN_STATE_CANCEL );

            this->finalize_();
        }

        if( cb != nullptr )
        {
            cb->onChain( m_state == TASK_CHAIN_STATE_SKIP, true );
        }

        this->decref();
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::skip()
    {
        this->incref();

        if( m_state == TASK_CHAIN_STATE_RUN )
        {
            this->skipRunningTasks_();
        }

        this->decref();
    }
    //////////////////////////////////////////////////////////////////////////
    bool Chain::isComplete() const
    {
        return m_complete;
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::runTask( const TaskPtr & _task )
    {
        m_runningTasks.emplace_back( _task );
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

        ChainProviderPtr cb = std::move( m_cb );

        this->finalize_();

        if( cb != nullptr )
        {
            cb->onChain( _skip, false );
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::skipRunningTasks_()
    {
        VectorTasks tasks = m_runningTasks;

        for( const TaskPtr & task : tasks )
        {
            task->skip();
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::cancelRunningTasks_()
    {
        VectorTasks tasks = m_runningTasks;

        for( const TaskPtr & task : tasks )
        {
            task->cancel( true );
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::finalize_()
    {
        m_source = nullptr;
        m_cb = nullptr;

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