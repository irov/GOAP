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
    typedef IntrusivePtr<class Factory> FactoryPtr;
	typedef IntrusivePtr<class Source> SourcePtr;

	typedef std::vector<SourcePtr> TVectorSources;

	class TranscriptorParallel
		: public Transcriptor
	{
	public:
		TranscriptorParallel( const FactoryPtr & _factory, size_t _count );

	public:
		TVectorSources & getSources();

	public:
		TaskPtr generate( const ChainPtr & _chain, const TaskPtr & _task ) override;

	protected:
		TVectorSources m_sources;
	};
}