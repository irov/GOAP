/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/TaskFork.h"

#include "GOAP/Macros.h"

#include "GOAP/Source.h"
#include "GOAP/Chain.h"
#include "GOAP/ChainProvider.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    namespace Detail
    {
        class ChainProviderTaskFork
            : public ChainProvider
        {
        public:
            ChainProviderTaskFork( const ChainPtr & _chain, const ChainPtr & _fork )
                : m_chain( _chain )
                , m_fork( _fork )
            {
            }

        protected:
            void onChain( bool _skip ) override
            {
                GOAP_UNUSED( _skip );

                m_chain->removeFork( m_fork );
            }

        protected:
            ChainPtr m_chain;
            ChainPtr m_fork;
        };
    }
    //////////////////////////////////////////////////////////////////////////
    TaskFork::TaskFork( const SourcePtr & _fork )
        : m_fork( _fork )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskFork::~TaskFork()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskFork::_onRun()
    {
        bool skip = this->isSkip();
        m_fork->setSkip( skip );

        ChainPtr chain = new Chain( m_fork );

        ChainProviderPtr chainProvider = new Detail::ChainProviderTaskFork( m_chain, chain );

        chain->setCallbackProvider( chainProvider );

        chain->run();

        m_chain->addFork( chain );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskFork::_onFinally()
    {
        m_fork = nullptr;
    }
}
