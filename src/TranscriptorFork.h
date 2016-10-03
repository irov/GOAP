#	pragma once

#	include "Transcriptor.h"

#	include <vector>

namespace GOAP
{
	typedef IntrusivePtr<class Source> SourcePtr;
	
	class TranscriptorFork
		: public Transcriptor
	{
	public:
		TranscriptorFork();

	public:
		const SourcePtr & getSource() const;

	public:
		TaskPtr generate( const ChainPtr & _chain, const TaskPtr & _task ) override;

	protected:
		SourcePtr m_source;
	};
}