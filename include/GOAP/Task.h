/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	pragma once

#   include "GOAP/Config.h"

namespace GOAP
{
	typedef IntrusivePtr<class Task> TaskPtr;
    typedef IntrusivePtr<class Source> SourcePtr;
	typedef IntrusivePtr<class Chain> ChainPtr;

	typedef Vector<TaskPtr> TVectorTasks;

    enum ETaskEvent
    {
        TASK_EVENT_INITIALIZE = 1 >> 0,
        TASK_EVENT_FINALIZE = 1 >> 1,
        TASK_EVENT_VALIDATE = 1 >> 2,
        TASK_EVENT_CHECK = 1 >> 3,
        TASK_EVENT_RUN = 1 >> 4,
        TASK_EVENT_SKIPABLE = 1 >> 5,
        TASK_EVENT_SKIP_NO_SKIPED = 1 >> 6,
        TASK_EVENT_SKIP_BLOCK = 1 >> 7,
        TASK_EVENT_COMPLETE = 1 >> 8,
        TASK_EVENT_FAST_SKIP = 1 >> 9,
        TASK_EVENT_SKIP = 1 >> 10,
        TASK_EVENT_CANCEL = 1 >> 11,
        TASK_EVENT_FINALLY = 1 >> 12,
        TASK_EVENT_CHECK_RUN = 1 >> 13,
        TASK_EVENT_CHECK_SKIP = 1 >> 14
    };

	class Task
		: public Factorable
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
        explicit Task( uint32_t _events );

    public:
		virtual ~Task();

    public:
        void setEvents( uint32_t _events );
        uint32_t getEvents() const;

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
        bool injectSource( const SourcePtr & _source );

	public:
		bool run( bool _checkSkipedFalse = false );
		bool skip();

		void cancel();

	public:
		void complete( bool _running = true, bool _skiped = false );

	protected:		
		bool onInitialize();
		void onFinalize();

	protected:
		bool onValidate() const;
		bool onCheck() const;
		bool onRun();
		bool onSkipable() const;
		void onSkipNoSkiped();
		bool onSkipBlock();
		void onComplete();
        bool onFastSkip();
		void onSkip();
		void onCancel();
		void onFinally();
		bool onCheckRun() const;
		bool onCheckSkip() const;

    protected:
        virtual bool _onInitialize();
        virtual void _onFinalize();

    protected:
        virtual bool _onValidate() const;
        virtual bool _onCheck() const;
        virtual bool _onRun();
        virtual bool _onSkipable() const;
        virtual void _onSkipNoSkiped();
        virtual bool _onSkipBlock();
        virtual void _onComplete();
        virtual bool _onFastSkip();
        virtual void _onSkip();
        virtual void _onCancel();
        virtual void _onFinally();
        virtual bool _onCheckRun() const;
        virtual bool _onCheckSkip() const;

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
		void setState( ETaskState _state );

	protected:
		ETaskState m_state;

        uint32_t m_events;

		ChainPtr m_chain;
		
		TVectorTasks m_nexts;
		TVectorTasks m_prevs;

		bool m_skip;
	};

	typedef IntrusivePtr<Task> TaskPtr;
}