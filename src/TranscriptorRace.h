/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Vector.h"

#include "GOAP/Transcriptor.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class Source> SourcePtr;
    //////////////////////////////////////////////////////////////////////////
    typedef Vector<SourcePtr> VectorSources;
    //////////////////////////////////////////////////////////////////////////
    class TranscriptorRace
        : public Transcriptor
    {
    public:
        explicit TranscriptorRace( const VectorSources & _sources );
        ~TranscriptorRace() override;

    public:
        const VectorSources & getSources() const;

    public:
        TaskPtr generate( const ChainPtr & _chain, const TaskPtr & _task ) override;

    protected:
        VectorSources m_sources;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TranscriptorRace> TranscriptorRacePtr;
    //////////////////////////////////////////////////////////////////////////
}