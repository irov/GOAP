/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/ChainInterface.h"
#include "GOAP/Vector.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class Node> NodePtr;
    //////////////////////////////////////////////////////////////////////////
    class Chain
        : public ChainInterface
    {
    public:
        enum ETaskChainState
        {
            TASK_CHAIN_STATE_IDLE,
            TASK_CHAIN_STATE_RUN,
            TASK_CHAIN_STATE_SKIP,
            TASK_CHAIN_STATE_COMPLETE,
            TASK_CHAIN_STATE_STOP_IDLE,
            TASK_CHAIN_STATE_STOP_RUN,
            TASK_CHAIN_STATE_CANCEL,
            TASK_CHAIN_STATE_FINALIZE,
        };

    public:
        explicit Chain( const SourceInterfacePtr & _source );
        ~Chain() override;

    public:
        template<class F>
        void setCallback( F _f )
        {
            ChainProviderPtr untilChainProvider = Helper::makeChainProvider( _f );

            this->setCallbackProvider( untilChainProvider );
        }

        void setCallbackProvider( const ChainProviderPtr & _cb ) override;
        const ChainProviderPtr & getCallbackProvider() const override;

    public:
        const SourceInterfacePtr & getSource() const override;

    public:
        bool run() override;
        void skip() override;
        void cancel() override;

    public:
        bool isComplete() const override;

    protected:
        void runNode( const NodePtr & _task );
        void completeNode( const NodePtr & _task );
        void processNode( const NodePtr & _task, bool _skip );

    protected:
        void complete( bool _skip );

    protected:
        void skipRunningNodes_();
        void cancelRunningNodes_();
        void finalize_();
        void setState_( ETaskChainState _state );
        ETaskChainState getState_() const;

    protected:
        SourceInterfacePtr m_source;

        ETaskChainState m_state;

        typedef Vector<NodePtr> VectorNodes;
        VectorNodes m_runningNodes;

        ChainProviderPtr m_cb;

        bool m_complete;

        friend Node;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<Chain> ChainPtr;
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        inline ChainPtr makeChain( const SourceInterfacePtr & _source )
        {
            return ChainPtr::from( new Chain( _source ) );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}