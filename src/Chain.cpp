/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "Chain.h"

#include "GOAP/ChainProviderInterface.h"
#include "GOAP/KernelInterface.h"
#include "GOAP/NodeInterface.h"
#include "GOAP/SourceProviderInterface.h"
#include "GOAP/SourceInterface.h"
#include "GOAP/FunctionContextProviderInterface.h"

#include "TaskDummy.h"
#include "TaskFunctionContext.h"

#include <algorithm>

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    Chain::Chain( KernelInterface * _kernel, const SourceInterfacePtr & _source, const char * _file, uint32_t _line )
        : m_kernel( _kernel )
        , m_source( _source )
        , m_file( _file )
        , m_line( _line )
        , m_state( TASK_CHAIN_STATE_IDLE )
        , m_cancel( false )
        , m_complete( false )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Chain::~Chain()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    KernelInterface * Chain::getKernel() const
    {
        return m_kernel;
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::setCallbackProvider( const ChainProviderInterfacePtr & _cb )
    {
        m_cb = _cb;
    }
    //////////////////////////////////////////////////////////////////////////
    const ChainProviderInterfacePtr & Chain::getCallbackProvider() const
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

        Allocator * allocator = this->getAllocator();

        FunctionContextProviderInterfacePtr context = Helper::makeFunctionContextProvider( allocator, [this]( bool _isSkip )
        {
            this->complete( _isSkip );
        } );

        TaskInterfacePtr provider_context = Helper::makeTask<TaskFunctionContext>( allocator, context );

        NodeInterfacePtr node_complete = m_kernel->makeNode( provider_context );

        m_source->addNode( node_complete );

        TaskInterfacePtr provider_dummy = Helper::makeTask<TaskDummy>( allocator );

        NodeInterfacePtr task_first = m_kernel->makeNode( provider_dummy );

        task_first->setChain( ChainInterfacePtr::from( this ) );

        const SourceProviderInterfacePtr & source_provider = m_source->getSourceProvider();

        source_provider->parse( ChainInterfacePtr::from( this ), task_first );

        bool skip = source_provider->isSkip();

        this->processNode( task_first, skip );

        m_source = nullptr;

        this->decref();

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::cancel()
    {
        m_cancel = true;

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

        ChainProviderInterfacePtr cb = std::move( m_cb );

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
    bool Chain::isCancel() const
    {
        return m_cancel;
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::runNode( const NodeInterfacePtr & _task )
    {
        m_runningNodes.emplace_back( _task );
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::completeNode( const NodeInterfacePtr & _task )
    {
        VectorNodes::iterator it_found = std::find( m_runningNodes.begin(), m_runningNodes.end(), _task );

        if( it_found == m_runningNodes.end() )
        {
            return;
        }

        m_runningNodes.erase( it_found );
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::processNode( const NodeInterfacePtr & _task, bool _skip )
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

        ChainProviderInterfacePtr cb = std::move( m_cb );

        if( m_cancel == false )
        {
            this->finalize_();
        }

        if( cb != nullptr )
        {
            cb->onChain( _skip, false );
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::skipRunningNodes_()
    {
        VectorNodes tasks = m_runningNodes;

        for( const NodeInterfacePtr & task : tasks )
        {
            task->skip();
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void Chain::cancelRunningNodes_()
    {
        VectorNodes tasks = m_runningNodes;

        for( const NodeInterfacePtr & task : tasks )
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