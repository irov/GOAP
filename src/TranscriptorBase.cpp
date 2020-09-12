/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TranscriptorBase.h"

#include "GOAP/NodeInterface.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TranscriptorBase::TranscriptorBase( const NodeInterfacePtr & _task )
        : m_task( _task )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TranscriptorBase::~TranscriptorBase()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    NodeInterfacePtr TranscriptorBase::generate( const ChainInterfacePtr & _chain, const NodeInterfacePtr & _task )
    {
        m_task->setChain( _chain );

        _task->addNext( m_task );

        return m_task;
    }
}