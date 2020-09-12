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
    class TranscriptorRace
        : public TranscriptorInterface
    {
    public:
        TranscriptorRace( VectorSources && _sources );
        ~TranscriptorRace() override;

    public:
        const VectorSources & getSources() const;

    public:
        NodeInterfacePtr generate( const ChainInterfacePtr & _chain, const NodeInterfacePtr & _task ) override;

    protected:
        VectorSources m_sources;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TranscriptorRace> TranscriptorRacePtr;
    //////////////////////////////////////////////////////////////////////////
}