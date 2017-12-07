/*
* Copyright (C) 2017, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	pragma once

#   include "GOAP/Vector.h"

#	include "Transcriptor.h"

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
		TranscriptorRace( size_t _count );
        ~TranscriptorRace() override;

	public:
		const VectorSources & getSources() const;

	public:
		TaskPtr generate( const ChainPtr & _chain, const TaskPtr & _task ) override;

	protected:
		VectorSources m_sources;
	};
}