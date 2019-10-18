/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/Chain.h"
#include "GOAP/Node.h"
#include "GOAP/Source.h"

#include "GOAP/ChainProvider.h"

#include "GOAP/TaskDummy.h"
#include "GOAP/TaskFunctionContext.h"

#include <algorithm>

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    Chain::Chain( const SourceInterfacePtr & _source )
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
    const SourceInterfacePtr & Chain::getSource() const
    {
        return m_source;
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

        FunctionContextProviderPtr context = Helper::makeFunctionContextProvider( [this]( bool _isSkip )
        {
            this->complete( _isSkip );
        } );

        TaskInterfacePtr provider_context = Helper::makeTask<TaskFunctionContext>( context );

        NodePtr task = m_source->makeNode( provider_context );

        m_source->addTask( task );

        TaskInterfacePtr provider_dummy = Helper::makeTask<TaskDummy>();

        NodePtr task_first = m_source->makeNode( provider_dummy );

        task_first->setChain( ChainPtr::from( this ) );

        const SourceProviderInterfacePtr & source_provider = m_source->getSourceProvider();

        source_provider->parse( ChainPtr::from( this ), task_first );

        bool skip = source_provider->isSkip();

        this->processNode( task_first, skip );

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
                this->skipRunningNodes_();
                this->cancelRunningNodes_();
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
            this->skipRunningNodes_();
        }

        this->decref();
    }
    //////////////////////////////////////////////////////////////////////////
    bool Chain::isComplete() const
    {
        return m_complete;
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::runNode( const NodePtr & _task )
    {
        m_runningNodes.emplace_back( _task );
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::completeNode( const NodePtr & _task )
    {
        VectorNodes::iterator it_found = std::find( m_runningNodes.begin(), m_runningNodes.end(), _task );

        if( it_found == m_runningNodes.end() )
        {
            return;
        }

        m_runningNodes.erase( it_found );
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::processNode( const NodePtr & _task, bool _skip )
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
    void Chain::skipRunningNodes_()
    {
        VectorNodes tasks = m_runningNodes;

        for( const NodePtr & task : tasks )
        {
            task->skip();
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::cancelRunningNodes_()
    {
        VectorNodes tasks = m_runningNodes;

        for( const NodePtr & task : tasks )
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