#	pragma once

#	include "GOAP/IntrusivePtrBase.h"

namespace GOAP
{
	typedef IntrusivePtr<class Task> TaskPtr;
	typedef IntrusivePtr<class Chain> ChainPtr;
	
	class Transcriptor
		: public IntrusivePtrBase<Transcriptor>
	{
	public:
		virtual ~Transcriptor() {};

	public:
		virtual TaskPtr generate( const ChainPtr & _chain, const TaskPtr & _task ) = 0;
	};

	typedef IntrusivePtr<Transcriptor> TranscriptorPtr;
}