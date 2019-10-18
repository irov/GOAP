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
        explicit TranscriptorBase( const NodePtr & _task );
        ~TranscriptorBase() override;

    public:
        NodePtr generate( const ChainPtr & _chain, const NodePtr & _task ) override;

    protected:
        NodePtr m_task;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TranscriptorBase> TranscriptorBasePtr;
    //////////////////////////////////////////////////////////////////////////
}