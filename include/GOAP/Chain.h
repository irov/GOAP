#	pragma once

#	include "GOAP/IntrusivePtrBase.h"

#	include <vector>

namespace GOAP
{
	class CallbackObserver;

	typedef IntrusivePtr<class Task> TaskPtr;
	typedef IntrusivePtr<class Source> SourcePtr;
	typedef IntrusivePtr<class ChainProvider> ChainProviderPtr;

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
		Chain( const SourcePtr & _source, const ChainProviderPtr & _cb );
		~Chain();

	public:
		bool run();
		void skip();
		void cancel();		
		
	public:
		bool isComplete() const;

	public:
		void runTask( const TaskPtr & _task );
		void completeTask( const TaskPtr & _task );
		void processTask( const TaskPtr & _task, bool _skip );

	protected:
		void complete( CallbackObserver * _callback, bool _skip );

	protected:
		void skipRunningTasks_();
		void cancelRunningTasks_();
		void finalize_();
		void setChain_( ETaskChainState _state );

	public:
		ETaskChainState m_state;

		SourcePtr m_source;

		TVectorTask m_runningTasks;

		ChainProviderPtr m_cb;

		bool m_complete;
	};

	typedef IntrusivePtr<Chain> ChainPtr;
}