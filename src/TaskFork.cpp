/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	include "GOAP/TaskFork.h"

#   include "GOAP/Macros.h"

#	include "GOAP/Source.h"
#	include "GOAP/Chain.h"
#	include "GOAP/ChainProvider.h"

namespace GOAP
{
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

        ChainPtr chain = GOAP_NEW Chain( m_fork );

        chain->setCallback( [this, chain]( bool _skip )
        { 
            GOAP_UNUSED( _skip );

            m_chain->removeFork( chain );
        } );

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
