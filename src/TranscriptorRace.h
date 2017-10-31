/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	pragma once

#	include "Transcriptor.h"

#	include <vector>

namespace GOAP
{
	typedef IntrusivePtr<class Source> SourcePtr;

	typedef std::vector<SourcePtr> TVectorSources;

	class TranscriptorRace
		: public Transcriptor
	{
	public:
		TranscriptorRace( size_t _count );

	public:
		const TVectorSources & getSources() const;

	public:
		TaskPtr generate( const ChainPtr & _chain, const TaskPtr & _task ) override;

	protected:
		TVectorSources m_sources;
	};
}