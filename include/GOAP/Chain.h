#	pragma once

#	include "IntrusivePtrBase.h"

#	include <vector>

namespace GOAP
{
	typedef IntrusivePtr<class Task> TaskPtr;
	typedef IntrusivePtr<class Source> TaskSourcePtr;
	typedef IntrusivePtr<class FunctionProvider> FunctionProviderPtr;

	typedef std::vector<TaskPtr> TVectorTask;

	class Chain
		: public IntrusivePtrBase<Chain>
	{
	public:
		enum ETaskChainState
		{
			TASK_CHAIN_STATE_IDLE,
			TASK_CHAIN_STATE_RUN,
			TASK_CHAIN_STATE_SKIP,
			TASK_CHAIN_STATE_COMPLETE,
			TASK_CHAIN_STATE_STOP_IDLE,
			TASK_CHAIN_STATE_STOP_RUN,
			TASK_CHAIN_STATE_CANCEL,
			TASK_CHAIN_STATE_FINALIZE,
		};

	public:
		Chain( const TaskSourcePtr & _source, const FunctionProviderPtr & _cb );
		~Chain();

	public:
		bool run();
		
	public:
		bool isComplete() const;

	public:
		void runTask( const TaskPtr & _task );
		void completeTask( const TaskPtr & _task );
		void processTask( const TaskPtr & _task, bool _skip );

	protected:
		void complete();

	public:
		ETaskChainState m_state;

		TaskSourcePtr m_source;

		TVectorTask m_runningTasks;

		FunctionProviderPtr m_cb;

		bool m_complete;
	};

	typedef IntrusivePtr<Chain> TaskChainPtr;
}