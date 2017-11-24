/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	pragma once

#	include "GOAP/Config.h"

#   include "GOAP/ChainProvider.h"

namespace GOAP
{
    class CallbackObserver;

    typedef IntrusivePtr<class Task> TaskPtr;
    typedef IntrusivePtr<class Source> SourcePtr;

    typedef Vector<TaskPtr> VectorTask;

    class Chain
        : public Factorable
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
        Chain( const SourcePtr & _source );
        ~Chain();

    public:
        template<class F>
        void addCallback( F _f )
        {
            ChainProviderPtr untilChainProvider = Helper::makeChainProvider<F>( _f );

            this->addCallbackProvider( untilChainProvider );
        }

        void addCallbackProvider( const ChainProviderPtr & _cb );

    public:
        bool run();
        void skip();
        void cancel();

    public:
        bool isComplete() const;

    public:
        void runTask( const TaskPtr & _task );
        void completeTask( const TaskPtr & _task );
        void processTask( const TaskPtr & _task, bool _skip );

    protected:
        void complete( CallbackObserver * _callback, bool _skip );

    protected:
        void skipRunningTasks_();
        void cancelRunningTasks_();
        void finalize_();
        void setState_( ETaskChainState _state );
        ETaskChainState getState_() const;

    public:
        ETaskChainState m_state;

        SourcePtr m_source;

        VectorTask m_runningTasks;

        ChainProviderPtr m_cb;

        bool m_complete;
    };

    typedef IntrusivePtr<Chain> ChainPtr;
}