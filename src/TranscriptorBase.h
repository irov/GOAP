/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/TranscriptorInterface.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class TranscriptorBase
        : public TranscriptorInterface
    {
    public:
        TranscriptorBase( Allocator * _allocator, const NodeInterfacePtr & _task );
        ~TranscriptorBase() override;

    public:
        NodeInterfacePtr generate( const ChainInterfacePtr & _chain, const NodeInterfacePtr & _task ) override;

    protected:
        NodeInterfacePtr m_task;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TranscriptorBase> TranscriptorBasePtr;
    //////////////////////////////////////////////////////////////////////////
}