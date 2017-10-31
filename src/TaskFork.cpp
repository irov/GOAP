/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	include "GOAP/TaskFork.h"
#	include "GOAP/Source.h"
#	include "GOAP/Chain.h"
#	include "GOAP/ChainProvider.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskFork::TaskFork( const SourcePtr & _fork )
        : Task( TASK_EVENT_RUN )
        , m_fork( _fork )
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

        chain->run();

        m_chain = chain;

        return true;
    }
}
