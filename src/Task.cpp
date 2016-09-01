#	include "GOAP/Task.h"
#	include "GOAP/Chain.h"

namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
	Task::Task()
		: m_state( TASK_STATE_IDLE )
		, m_skip( false )
	{
	}
	//////////////////////////////////////////////////////////////////////////
	Task::~Task()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	void Task::setChain( const ChainPtr & _chain )
	{
		m_chain = _chain;
	}
	//////////////////////////////////////////////////////////////////////////
	const ChainPtr & Task::getChain() const
	{
		return m_chain;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Task::isIdle() const
	{
		return m_state == TASK_STATE_IDLE;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Task::isRunning() const
	{
		return m_state == TASK_STATE_RUN;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Task::isEnd() const
	{
		return m_state == TASK_STATE_END;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Task::isSkip() const
	{
		return m_skip;
	}
	//////////////////////////////////////////////////////////////////////////
	void Task::addNext( const TaskPtr & _task )
	{
		m_nexts.push_back( _task );
		_task->addPrev_( this );
	}
	//////////////////////////////////////////////////////////////////////////
	TVectorTasks & Task::getNexts()
	{
		return m_nexts;
	}
	//////////////////////////////////////////////////////////////////////////
	const TVectorTasks & Task::getNexts() const
	{
		return m_nexts;
	}
	//////////////////////////////////////////////////////////////////////////
	void Task::popNexts( TVectorTasks & _clone )
	{
		for( TVectorTasks::iterator
			it = m_nexts.begin(),
			it_end = m_nexts.end();
		it != it_end;
		++it )
		{
			const TaskPtr & next = *it;

			next->removePrev_( this );
		}

		_clone.swap( m_nexts );

		m_nexts.clear();
	}
	//////////////////////////////////////////////////////////////////////////
	bool Task::run( bool _checkSkipedFalse )
	{
		if( m_state != TASK_STATE_IDLE )
		{
			return false;
		}

		if( this->onValidate() == false )
		{
			return false;
		}

		this->setState( TASK_STATE_RUN );

		m_chain->runTask( this );

		if( this->onInitialize() == false )
		{
			return false;
		}

		if( this->onCheck() == false )
		{
			this->complete( false );

			return true;
		}

		bool done = this->onRun();

		if( m_state != TASK_STATE_RUN )
		{
			return true;
		}

		if( done == false )
		{
			return true;
		}

		if( _checkSkipedFalse == true )
		{
			if( this->onSkipBlock() == true )
			{
				this->complete( true, false );

				return true;
			}

			if( this->onSkipable() == false )
			{
				return false;
			}
		}

		this->complete( true, false );
	
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Task::skip()
	{
		if( m_state == TASK_STATE_SKIP )
		{
			return true;
		}

		if( m_state == TASK_STATE_END )
		{
			return true;
		}

		if( m_skip == true )
		{
			return false;
		}

		if( this->onSkipable() == false )
		{
			if( m_state == TASK_STATE_IDLE )
			{
				if( this->run( true ) == false )
				{
					return false;
				}
			}
			else
			{
				this->onSkipNoSkiped();
			}
		}

		m_skip = true;

		switch( m_state )
		{
		case TASK_STATE_IDLE:
			{
				if( this->onInitialize() == false )
				{
					return false;
				}

				m_chain->runTask( this );

				this->setState( TASK_STATE_RUN );

				if( this->onCheck() == true )
				{
					bool done = this->onRun();

					if( m_state == TASK_STATE_END )
					{
						return true;
					}

					this->setState( TASK_STATE_SKIP );

					if( done == false )
					{
						this->onSkip();
					}
				}

				this->onFinally();

				this->taskSkip_();
			}break;
		case TASK_STATE_RUN:
			{
				this->setState( TASK_STATE_SKIP );

				this->onSkip();
				this->onFinally();

				this->taskSkip_();
			}break;
		default:
			break;
		}

		if( m_state != TASK_STATE_END )
		{
			this->finalize_();
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void Task::cancel()
	{
		if( m_state == TASK_STATE_END )
		{
			return;
		}

		m_skip = true;

		if( m_state == TASK_STATE_RUN )
		{
			this->setState( TASK_STATE_CANCEL );

			this->onSkip();
			this->onCancel();
			this->onFinally();

			m_chain->completeTask( this );
		}

		this->finalize_();
	}
	//////////////////////////////////////////////////////////////////////////
	void Task::taskSkip_()
	{
		if( m_state == TASK_STATE_END )
		{
			return;
		}

		m_chain->completeTask( this );

		TVectorTasks copy_nexts = m_nexts;

		for( TVectorTasks::iterator
			it = copy_nexts.begin(),
			it_end = copy_nexts.end();
		it != it_end;
		++it )
		{
			const TaskPtr & next = *it;

			if( next->prevSkip_( this ) == true )
			{
				m_chain->processTask( next, true );
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Task::complete( bool _running, bool _skiped )
	{
		switch( m_state )
		{
		case TASK_STATE_SKIP:
		case TASK_STATE_END:
		case TASK_STATE_CANCEL:
			return;
		default:
			break;
		};

		if( m_state != TASK_STATE_RUN )
		{
			return;
		}

		if( _skiped == true )
		{
			m_skip = true;
			this->setState( TASK_STATE_SKIP );
		}
		else
		{
			this->setState( TASK_STATE_COMPLETE );
		}

		if( _running == true )
		{
			this->onComplete();
			this->onFinally();
		}

		this->setState( TASK_STATE_END );

		if( m_skip == false )
		{
			TVectorTasks copy_nexts = m_nexts;

			for( TVectorTasks::iterator
				it = copy_nexts.begin(),
				it_end = copy_nexts.end();
			it != it_end;
			++it )
			{
				const TaskPtr & next = *it;

				if( next->prevComplete_( this ) == true )
				{
					m_chain->processTask( next, false );
				}
			}
		}
		else
		{
			TVectorTasks copy_nexts = m_nexts;

			for( TVectorTasks::iterator
				it = copy_nexts.begin(),
				it_end = copy_nexts.end();
			it != it_end;
			++it )
			{
				const TaskPtr & next = *it;

				if( next->prevSkip_( this ) == true )
				{
					m_chain->processTask( next, true );
				}
			}
		}

		ChainPtr chain = m_chain;

		this->finalize_();

		chain->completeTask( this );
	}
	//////////////////////////////////////////////////////////////////////////
	bool Task::prevSkip_( Task * _task )
	{
		if( m_state == TASK_STATE_END )
		{
			return false;
		}

#	ifdef _DEBUG
		if( this->hasPrev_( _task ) == false )
		{
			return false;
		}
#	endif

		this->removePrev_( _task );

		if( this->onCheckSkip() == false )
		{
			return false;
		}

		this->cancelPrev_();

		if( m_state == TASK_STATE_END )
		{
			return false;
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void Task::skipPrev_()
	{
		TVectorTasks clone_prevs = m_prevs;

		for( TVectorTasks::iterator
			it = clone_prevs.begin(),
			it_end = clone_prevs.end();
		it != it_end;
		++it )
		{
			const TaskPtr & prev = *it;

			switch( prev->m_state )
			{
			case TASK_STATE_IDLE:
				{
					prev->skipPrev_();
					prev->cancelPrev_();
				}break;
			case  TASK_STATE_RUN:
				{
					prev->skip();
					prev->cancel();
				}break;
			default:
				break;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Task::cancelPrev_()
	{
		TVectorTasks clone_prevs = m_prevs;

		for( TVectorTasks::iterator
			it = clone_prevs.begin(),
			it_end = clone_prevs.end();
		it != it_end;
		++it )
		{
			const TaskPtr & prev = *it;

			switch( prev->m_state )
			{
			case TASK_STATE_IDLE:
				{
					prev->cancelPrev_();
					prev->cancel();
				}break;
			case  TASK_STATE_RUN:
				{
					prev->cancel();
				}break;
			default:
				break;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	bool Task::prevComplete_( Task *_task )
	{
		if( m_state != TASK_STATE_IDLE )
		{
			return false;
		}

#	ifdef _DEBUG
		if( this->hasPrev_( _task ) == false )
		{
			return false;
		}
#	endif
		this->removePrev_( _task );

		if( this->onCheckRun() == false )
		{
			return false;
		}

		this->cancelPrev_();

		if( m_state == TASK_STATE_END )
		{
			return false;
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Task::onValidate() const
	{
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Task::onInitialize()
	{
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void Task::onFinalize()
	{
		//Empty
	}
	//////////////////////////////////////////////////////////////////////////
	bool Task::onCheck() const
	{
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Task::onRun()
	{
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Task::onSkipable() const
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	void Task::onSkipNoSkiped()
	{
		//Empty
	}
	//////////////////////////////////////////////////////////////////////////
	bool Task::onSkipBlock()
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	void Task::onComplete()
	{
		//Empty
	}
	//////////////////////////////////////////////////////////////////////////
	void Task::onSkip()
	{
		//Empty
	}
	//////////////////////////////////////////////////////////////////////////
	void Task::onCancel()
	{
		//Empty
	}
	//////////////////////////////////////////////////////////////////////////
	void Task::onFinally()
	{
		//Empty
	}
	//////////////////////////////////////////////////////////////////////////
	bool Task::onCheckRun() const
	{
		if( m_prevs.empty() == true )
		{
			return true;
		}

		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Task::onCheckSkip() const
	{
		if( m_prevs.empty() == true )
		{
			return true;
		}

		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	void Task::addPrev_( Task * _task )
	{
		m_prevs.push_back( _task );
	}
	//////////////////////////////////////////////////////////////////////////
	void Task::removePrev_( Task * _task )
	{
		TVectorTasks::iterator it_erase = std::find( m_prevs.begin(), m_prevs.end(), _task );

		m_prevs.erase( it_erase );
	}
	//////////////////////////////////////////////////////////////////////////
	bool Task::hasPrev_( const Task * _task ) const
	{
		TVectorTasks::const_iterator it_found = std::find( m_prevs.begin(), m_prevs.end(), _task );

		return it_found != m_prevs.end();
	}
	//////////////////////////////////////////////////////////////////////////
	void Task::finalize_()
	{
		this->setState( TASK_STATE_END );

		this->onFinalize();

		m_chain = nullptr;
		m_nexts.clear();
		m_prevs.clear();
	}
	//////////////////////////////////////////////////////////////////////////
	void Task::setState( ETaskState _state )
	{
		m_state = _state;
	}
}