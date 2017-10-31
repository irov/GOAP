/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	include "TranscriptorBase.h"

#	include "GOAP/Task.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TranscriptorBase::TranscriptorBase( const TaskPtr & _task )
        : m_task( _task )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskPtr TranscriptorBase::generate( const ChainPtr & _chain, const TaskPtr & _task )
    {
        m_task->setChain( _chain );

        _task->addNext( m_task );

        return m_task;
    }
}