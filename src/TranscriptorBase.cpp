/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TranscriptorBase.h"

#include "GOAP/Node.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TranscriptorBase::TranscriptorBase( const NodePtr & _task )
        : m_task( _task )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TranscriptorBase::~TranscriptorBase()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    NodePtr TranscriptorBase::generate( const ChainPtr & _chain, const NodePtr & _task )
    {
        m_task->setChain( _chain );

        _task->addNext( m_task );

        return m_task;
    }
}