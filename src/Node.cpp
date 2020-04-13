/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "Node.h"

#include "GOAP/SourceInterface.h"
#include "GOAP/ChainInterface.h"

#include "TaskDummy.h"

#include <algorithm>

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    Node::Node( const TaskInterfacePtr & _provider )
        : m_provider( _provider )
        , m_state( TASK_STATE_IDLE )
        , m_skip( false )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Node::~Node()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::setChain( const ChainInterfacePtr & _chain )
    {
        m_chain = _chain;
    }
    //////////////////////////////////////////////////////////////////////////
    const ChainInterfacePtr & Node::getChain() const
    {
        return m_chain;
    }
    //////////////////////////////////////////////////////////////////////////
    SourceInterfacePtr Node::makeSource()
    {
        const SourceInterfacePtr & chain_source = m_chain->getSource();

        SourceInterfacePtr source = chain_source->makeSource();

        const SourceProviderInterfacePtr & provider = source->getSourceProvider();

        bool skip = this->isSkip();
        provider->setSkip( skip );

        return source;
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::throwError()
    {
        m_state = TASK_STATE_ERROR;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Node::isIdle() const
    {
        return m_state == TASK_STATE_IDLE;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Node::isRunning() const
    {
        return m_state == TASK_STATE_RUN;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Node::isEnd() const
    {
        return m_state == TASK_STATE_END;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Node::isSkip() const
    {
        return m_skip;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Node::isError() const
    {
        return m_state == TASK_STATE_END;
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::addNext( const NodeInterfacePtr & _task )
    {
        m_nexts.emplace_back( _task );
        _task->addPrev( NodeInterfacePtr::from( this ) );
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::popNexts( VectorNodes & _clone )
    {
        for( const NodeInterfacePtr & next : m_nexts )
        {
            next->removePrev( NodeInterfacePtr::from( this ) );
        }

        _clone.swap( m_nexts );
    }
    //////////////////////////////////////////////////////////////////////////
    bool Node::injectSource( const SourceProviderInterfacePtr & _provider )
    {
        if( _provider->empty() == true )
        {
            return true;
        }

        VectorNodes nexts;
        this->popNexts( nexts );

        const ChainInterfacePtr & chain = this->getChain();

        NodeInterfacePtr task = _provider->parse( chain, NodeInterfacePtr::from( this ) );

        if( task == nullptr )
        {
            return false;
        }

        for( const NodeInterfacePtr & next : nexts )
        {
            task->addNext( next );
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Node::forkSource( const SourceProviderInterfacePtr & _provider )
    {
        if( _provider->empty() == true )
        {
            return true;
        }

        const ChainInterfacePtr & chain = this->getChain();

        NodeInterfacePtr node = _provider->parse( chain, NodeInterfacePtr::from( this ) );

        if( node == nullptr )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Node::run( bool _checkSkipedFalse )
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

        m_chain->runNode( NodeInterfacePtr::from( this ) );

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
    bool Node::skip()
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

                m_chain->runNode( NodeInterfacePtr::from( this ) );

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
    void Node::cancel( bool _withNexts )
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

            m_chain->completeNode( NodeInterfacePtr::from( this ) );
        }

        if( _withNexts == true )
        {
            VectorNodes copy_nexts = m_nexts;

            for( const NodeInterfacePtr & node : copy_nexts )
            {
                node->cancel( _withNexts );
            }
        }

        this->finalize_();
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::taskSkip_()
    {
        if( m_state == TASK_STATE_END )
        {
            return;
        }

        m_chain->completeNode( NodeInterfacePtr::from( this ) );

        VectorNodes copy_nexts = std::move( m_nexts );

        for( const NodePtr & next : copy_nexts )
        {
            if( next->prevSkip_( NodePtr::from( this ) ) == true )
            {
                m_chain->processNode( next, true );
            }
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::complete( bool _running, bool _skiped )
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
            VectorNodes copy_nexts = std::move( m_nexts );

            for( const NodePtr & next : copy_nexts )
            {
                if( next->prevComplete_( NodePtr::from( this ) ) == true )
                {
                    m_chain->processNode( next, false );
                }
            }
        }
        else
        {
            VectorNodes copy_nexts = std::move( m_nexts );

            for( const NodePtr & next : copy_nexts )
            {
                if( next->prevSkip_( NodePtr::from( this ) ) == true )
                {
                    m_chain->processNode( next, true );
                }
            }
        }

        ChainInterfacePtr chain = m_chain;

        this->finalize_();

        chain->completeNode( NodeInterfacePtr::from( this ) );
    }
    //////////////////////////////////////////////////////////////////////////
    bool Node::prevSkip_( const NodePtr & _task )
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
    void Node::skipPrev_()
    {
        VectorNodes clone_prevs = m_prevs;

        for( const NodePtr & prev : clone_prevs )
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
    void Node::cancelPrev_()
    {
        VectorNodes clone_prevs = m_prevs;

        for( const NodePtr & prev : clone_prevs )
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
    bool Node::prevComplete_( const NodePtr & _task )
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
    bool Node::isEmptyPrevs() const
    {
        bool result = m_prevs.empty();

        return result;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Node::onInitialize()
    {
        bool successful = m_provider->_onInitialize();

        return successful;
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::onFinalize()
    {
        m_provider->_onFinalize();
    }
    //////////////////////////////////////////////////////////////////////////
    bool Node::onValidate() const
    {
        bool valid = m_provider->_onValidate();

        return valid;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Node::onCheck()
    {
        bool check = m_provider->_onCheck();

        return check;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Node::onRun()
    {
        bool successful = m_provider->_onRun( this );

        return successful;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Node::onSkipable() const
    {
        bool result = m_provider->_onSkipable();

        return result;
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::onSkipNoSkiped()
    {
        m_provider->_onSkipNoSkiped();
    }
    //////////////////////////////////////////////////////////////////////////
    bool Node::onSkipBlock()
    {
        bool result = m_provider->_onSkipBlock();

        return result;
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::onComplete()
    {
        m_provider->_onComplete();
    }
    //////////////////////////////////////////////////////////////////////////
    bool Node::onFastSkip()
    {
        bool result = m_provider->_onFastSkip();

        return result;
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::onSkip()
    {
        m_provider->_onSkip();
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::onCancel()
    {
        m_provider->_onCancel();
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::onFinally()
    {
        m_provider->_onFinally();
    }
    //////////////////////////////////////////////////////////////////////////
    bool Node::onCheckRun() const
    {
        bool result = m_provider->_onCheckRun( this );

        return result;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Node::onCheckSkip() const
    {
        bool result = m_provider->_onCheckSkip( this );

        return result;
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::addPrev( const NodeInterfacePtr & _task )
    {
        m_prevs.emplace_back( _task );
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::removePrev( const NodeInterfacePtr & _task )
    {
        VectorNodes::iterator it_erase = std::find( m_prevs.begin(), m_prevs.end(), _task );

        m_prevs.erase( it_erase );
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::removeNext( const NodeInterfacePtr & _task )
    {
        VectorNodes::iterator it_erase = std::find( m_nexts.begin(), m_nexts.end(), _task );

        if( it_erase == m_nexts.end() )
        {
            return;
        }

        m_nexts.erase( it_erase );
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::unlink_( const NodeInterfacePtr & _task )
    {
        this->removePrev( _task );
        _task->removeNext( NodeInterfacePtr::from( this ) );
    }
    //////////////////////////////////////////////////////////////////////////
    bool Node::hasPrev_( const NodeInterfacePtr & _task ) const
    {
        VectorNodes::const_iterator it_found = std::find( m_prevs.begin(), m_prevs.end(), _task );

        return it_found != m_prevs.end();
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::finalize_()
    {
        this->setState( TASK_STATE_END );

        this->onFinalize();

        m_chain = nullptr;

        m_nexts.clear();
        m_prevs.clear();
    }
    //////////////////////////////////////////////////////////////////////////
    void Node::setState( ETaskState _state )
    {
        m_state = _state;
    }
}