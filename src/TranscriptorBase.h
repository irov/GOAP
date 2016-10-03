#	pragma once

#	include "Transcriptor.h"

namespace GOAP
{
	class TranscriptorBase
		: public Transcriptor
	{
	public:
		TranscriptorBase( const TaskPtr & _task );

	public:
		TaskPtr generate( const ChainPtr & _chain, const TaskPtr & _task ) override;

	protected:
		TaskPtr m_task;
	};
}