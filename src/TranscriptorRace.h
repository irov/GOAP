/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Vector.h"

#include "GOAP/TranscriptorInterface.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class SourceInterface> SourceInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    typedef Vector<SourceInterfacePtr> VectorSources;
    //////////////////////////////////////////////////////////////////////////
    class TranscriptorRace
        : public TranscriptorInterface
    {
    public:
        explicit TranscriptorRace( VectorSources && _sources );
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