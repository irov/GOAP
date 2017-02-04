#	pragma once

#	include "GOAP/Task.h"

namespace GOAP
{
	typedef IntrusivePtr<class Source> SourcePtr;
	typedef IntrusivePtr<class Chain> ChainPtr;

	class TaskFork
		: public Task
	{
	public:
		TaskFork( const SourcePtr & _fork );
		~TaskFork();

	public:
		bool _onRun() override;

	protected:
		SourcePtr m_fork;
		ChainPtr m_chain;
	};
}