#	include "GOAP/Chain.h"
#	include "GOAP/Task.h"
#	include "GOAP/Source.h"

#	include "GOAP/ChainProvider.h"
#	include "GOAP/CallbackProvider.h"

#	include "GOAP/TaskDummy.h"
#	include "GOAP/TaskCallback.h"

#	include <algorithm>

namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
	Chain::Chain( const SourcePtr & _source, const ChainProviderPtr & _cb )
		: m_source( _source )
		, m_cb( _cb )
		, m_state( TASK_CHAIN_STATE_IDLE )
		, m_complete( false )
	{
	}
	//////////////////////////////////////////////////////////////////////////
	Chain::~Chain()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	bool Chain::run()
	{
		this->setChain_( TASK_CHAIN_STATE_RUN );

		TaskPtr task_first = new TaskDummy();
		task_first->setChain( this );

		TaskPtr task_last = m_source->parse( this, task_first );

		CallbackProviderPtr provider = makeCallbackProvider( [this] ( CallbackObserver * _callback, bool _skip ) { this->complete( _callback, _skip ); } );

		TaskPtr task_cb = new TaskCallback( provider );
		task_cb->setChain( this );

		task_last->addNext( task_cb );

		this->processTask( task_first, false );

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void Chain::cancel()
	{
		if( m_state != TASK_CHAIN_STATE_IDLE &&
			m_state != TASK_CHAIN_STATE_CANCEL &&
			m_state != TASK_CHAIN_STATE_FINALIZE )
		{
			if( m_state == TASK_CHAIN_STATE_RUN )
			{
				this->skipRunningTasks_();
				this->cancelRunningTasks_();
			}

			//if( m_state != TASK_CHAIN_STATE_COMPLETE &&
			//	m_state != TASK_CHAIN_STATE_CANCEL &&
			//	m_state != TASK_CHAIN_STATE_FINALIZE )
			//{

			//}
		}

		if( m_state != TASK_CHAIN_STATE_CANCEL &&
			m_state != TASK_CHAIN_STATE_FINALIZE )
		{
			this->setChain_( TASK_CHAIN_STATE_CANCEL );

			this->finalize_();
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Chain::skip()
	{
		if( m_state == TASK_CHAIN_STATE_RUN )
		{
			this->skipRunningTasks_();
		}
	}
	//////////////////////////////////////////////////////////////////////////
	bool Chain::isComplete() const
	{
		return m_complete;
	}
	//////////////////////////////////////////////////////////////////////////
	void Chain::runTask( const TaskPtr & _task )
	{
		m_runningTasks.push_back( _task );
	}
	//////////////////////////////////////////////////////////////////////////
	void Chain::completeTask( const TaskPtr & _task )
	{
		TVectorTask::iterator it_found = std::find( m_runningTasks.begin(), m_runningTasks.end(), _task );

		if( it_found == m_runningTasks.end() )
		{
			return;
		}

		m_runningTasks.erase( it_found );
	}
	//////////////////////////////////////////////////////////////////////////
	void Chain::processTask( const TaskPtr & _task, bool _skip )
	{
		if( _skip == true )
		{
			_task->skip();
		}
		else
		{
			_task->run( false );
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Chain::complete( CallbackObserver * _callback, bool _skip )
	{
		_callback->onCallback( _skip );

		m_complete = true;
		this->setChain_( TASK_CHAIN_STATE_COMPLETE );

		ChainProviderPtr cb = m_cb;

		this->finalize_();
				
		if( cb != nullptr )
		{
			cb->onChain( _skip );
		}						
	}
	//////////////////////////////////////////////////////////////////////////
	void Chain::skipRunningTasks_()
	{
		TVectorTask tasks = m_runningTasks;
		
		for( TVectorTask::iterator
			it = tasks.begin(),
			it_end = tasks.end();
		it != it_end;
		++it )
		{
			const TaskPtr & task = *it;

			task->skip();
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Chain::cancelRunningTasks_()
	{
		TVectorTask tasks = m_runningTasks;

		for( TVectorTask::iterator
			it = tasks.begin(),
			it_end = tasks.end();
		it != it_end;
		++it )
		{
			const TaskPtr & task = *it;

			task->cancel();
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Chain::finalize_()
	{
		m_source = nullptr;
		//m_runningTasks.clear();

		this->setChain_( TASK_CHAIN_STATE_FINALIZE );
	}
	//////////////////////////////////////////////////////////////////////////
	void Chain::setChain_( ETaskChainState _state )
	{
		m_state = _state;
	}
}