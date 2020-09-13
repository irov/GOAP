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
        Chain( const SourceInterfacePtr & _source, const char * _file, uint32_t _line );
        ~Chain() override;

    public:
        template<class F>
        void setCallback( F _f )
        {
            Allocator * allocator = this->getAllocator();

            ChainProviderInterfacePtr untilChainProvider = Helper::makeChainProvider( allocator, _f );

            this->setCallbackProvider( untilChainProvider );
        }

        void setCallbackProvider( const ChainProviderInterfacePtr & _cb ) override;
        const ChainProviderInterfacePtr & getCallbackProvider() const override;

    public:
        const SourceInterfacePtr & getSource() const override;

    public:
        bool run() override;
        void skip() override;
        void cancel() override;

    public:
        bool isComplete() const override;
        bool isCancel() const override;

    protected:
        void runNode( const NodeInterfacePtr & _task ) override;
        void completeNode( const NodeInterfacePtr & _task ) override;
        void processNode( const NodeInterfacePtr & _task, bool _skip ) override;

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

        const char * m_file;
        uint32_t m_line;

        ETaskChainState m_state;

        typedef Vector<NodeInterfacePtr> VectorNodes;
        VectorNodes m_runningNodes;

        ChainProviderInterfacePtr m_cb;

        bool m_cancel;
        bool m_complete;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<Chain> ChainPtr;
    //////////////////////////////////////////////////////////////////////////
}