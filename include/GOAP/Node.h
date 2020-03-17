/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/NodeInterface.h"
#include "GOAP/TaskInterface.h"
#include "GOAP/Vector.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class Node> NodePtr;
    //////////////////////////////////////////////////////////////////////////
    class Node
        : public NodeInterface
    {
    public:
        enum ETaskState
        {
            TASK_STATE_INVALID,
            TASK_STATE_IDLE,
            TASK_STATE_RUN,
            TASK_STATE_SKIP,
            TASK_STATE_STOP,
            TASK_STATE_CANCEL,
            TASK_STATE_COMPLETE,
            TASK_STATE_ERROR,
            TASK_STATE_END
        };

    public:
        Node( const TaskInterfacePtr & _task );
        ~Node() override;

    public:
        void setChain( const ChainInterfacePtr & _chain ) override;
        const ChainInterfacePtr & getChain() const override;

    public:
        SourceInterfacePtr makeSource() override;

    public:
        void throwError() override;

    public:
        bool isIdle() const override;
        bool isRunning() const override;
        bool isEnd() const override;
        bool isSkip() const override;
        bool isError() const override;

    public:
        void addNext( const NodeInterfacePtr & _task ) override;
        void removeNext( const NodeInterfacePtr & _task ) override;

        void addPrev( const NodeInterfacePtr & _task ) override;
        void removePrev( const NodeInterfacePtr & _task ) override;

    public:
        bool injectSource( const SourceProviderInterfacePtr & _provider ) override;
        bool forkSource( const SourceProviderInterfacePtr & _provider ) override;

    public:
        bool run( bool _checkSkipedFalse = false ) override;
        bool skip() override;

        void cancel( bool _withNexts = false ) override;

    public:
        void complete( bool _running = true, bool _skiped = false ) override;

    public:
        bool isEmptyPrevs() const override;

    protected:
        bool onInitialize();
        void onFinalize();

    protected:
        bool onValidate() const;
        bool onCheck();
        bool onRun();
        bool onSkipable() const;
        void onSkipNoSkiped();
        bool onSkipBlock();
        void onComplete();
        bool onFastSkip();
        void onSkip();
        void onCancel();
        void onFinally();
        bool onCheckRun() const;
        bool onCheckSkip() const;

    protected:
        void taskSkip_();

        bool prevSkip_( const NodePtr & _task );
        void skipPrev_();
        void cancelPrev_();

        bool prevComplete_( const NodePtr & _task );

        void unlink_( const NodeInterfacePtr & _task );
        bool hasPrev_( const NodeInterfacePtr & _task ) const;
        void finalize_();

    protected:
        void setState( ETaskState _state );

    protected:
        TaskInterfacePtr m_provider;

        ETaskState m_state;

        ChainInterfacePtr m_chain;

        typedef Vector<NodeInterfacePtr> VectorNodes;
        VectorNodes m_nexts;
        VectorNodes m_prevs;

        bool m_skip;

    protected:
        void popNexts( VectorNodes & _clone );
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<Node> NodePtr;
    //////////////////////////////////////////////////////////////////////////
}