#	pragma once

#	include "GOAP/IntrusivePtrBase.h"

#	include <vector>

namespace GOAP
{
	typedef IntrusivePtr<class Task> TaskPtr;
	typedef IntrusivePtr<class Chain> ChainPtr;

	typedef std::vector<TaskPtr> TVectorTasks;

	class Task
		: public IntrusivePtrBase<Task>
	{
	public:
		enum ETaskState
		{
			TASK_STATE_INVALID,
			TASK_STATE_IDLE,
			TASK_STATE_RUN,
			TASK_STATE_SKIP,
			TASK_STATE_STOP,
			TASK_STATE_CANCEL,
			TASK_STATE_COMPLETE,
			TASK_STATE_END
		};
		
	public:
		Task();
		virtual ~Task();

	public:
		void setChain( const ChainPtr & _chain );
		const ChainPtr & getChain() const;

	public:
		bool isIdle() const;
		bool isRunning() const;
		bool isEnd() const;
		bool isSkip() const;

	public:
		void addNext( const TaskPtr & _task );
		TVectorTasks & getNexts();
		const TVectorTasks & getNexts() const;
		void popNexts( TVectorTasks & _clone );

	public:
		bool run( bool _checkSkipedFalse = false );
		bool skip();

		void cancel();

	public:
		void complete( bool _running = true, bool _skiped = false );

	protected:		
		virtual bool onInitialize();
		virtual void onFinalize();

	protected:
		virtual bool onValidate() const;
		virtual bool onCheck() const;
		virtual bool onRun();
		virtual bool onSkipable() const;
		virtual void onSkipNoSkiped();
		virtual bool onSkipBlock();
		virtual void onComplete();
		virtual void onSkip();
		virtual void onCancel();
		virtual void onFinally();
		virtual bool onCheckRun() const;
		virtual bool onCheckSkip() const;

	protected:
		void taskSkip_();

		bool prevSkip_( Task * _task );
		void skipPrev_();
		void cancelPrev_();

		bool prevComplete_( Task *_task );

		void addPrev_( Task * _task );
		void removePrev_( Task * _task );
		bool hasPrev_( const Task * _task ) const;
		void finalize_();

	protected:
		ETaskState m_state;

		ChainPtr m_chain;
		
		TVectorTasks m_nexts;
		TVectorTasks m_prevs;

		bool m_skip;
	};

	typedef IntrusivePtr<Task> TaskPtr;
}