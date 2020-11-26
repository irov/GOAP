/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/TranscriptorInterface.h"

#include "GOAP/VectorSources.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class SourceInterface> SourceInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    class TranscriptorParallel
        : public TranscriptorInterface
    {
    public:
        TranscriptorParallel( Allocator * _allocator, VectorSources && _sources );
        ~TranscriptorParallel() override;

    public:
        const VectorSources & getSources() const;

    public:
        NodeInterfacePtr generate( const ChainInterfacePtr & _chain, const NodeInterfacePtr & _task ) override;

    protected:
        VectorSources m_sources;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TranscriptorParallel> TranscriptorParallelPtr;
    //////////////////////////////////////////////////////////////////////////
}