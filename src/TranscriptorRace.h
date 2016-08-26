#	pragma once

#	include "Transcriptor.h"

#	include <vector>

namespace GOAP
{
	typedef IntrusivePtr<class Source> TaskSourcePtr;

	typedef std::vector<TaskSourcePtr> TVectorSources;

	class TranscriptorRace
		: public Transcriptor
	{
	public:
		TranscriptorRace( size_t _count );

	public:
		TVectorSources & getSources();

	public:
		TaskPtr generate( const TaskChainPtr & _chain, const TaskPtr & _task ) override;

	protected:
		TVectorSources m_sources;
	};
}