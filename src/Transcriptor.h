#	pragma once

#	include "GOAP/Config.h"

namespace GOAP
{
	typedef IntrusivePtr<class Task> TaskPtr;
	typedef IntrusivePtr<class Chain> ChainPtr;
	
	class Transcriptor
		: public IntrusiveBase<Transcriptor>
	{
	public:
		virtual ~Transcriptor() {};

	public:
		virtual TaskPtr generate( const ChainPtr & _chain, const TaskPtr & _task ) = 0;
	};

	typedef IntrusivePtr<Transcriptor> TranscriptorPtr;
}