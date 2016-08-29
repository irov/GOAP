#	include "GOAP/Chain.h"
#	include "GOAP/Task.h"
#	include "GOAP/Source.h"

#	include "GOAP/FunctionProvider.h"

#	include "GOAP/TaskDummy.h"
#	include "GOAP/TaskFunction.h"

#	include <algorithm>

namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
	Chain::Chain( const SourcePtr & _source, const FunctionProviderPtr & _cb )
		: m_source(_source)
		, m_cb(_cb)
		, m_complete(false)
	{
	}
	//////////////////////////////////////////////////////////////////////////
	Chain::~Chain()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	bool Chain::run()
	{
		TaskPtr task_first = new TaskDummy();
		task_first->setChain( this );

		TaskPtr task_last = m_source->parse( this, task_first );

		FunctionProviderPtr provider = makeFunctionProvider( [this] () { this->complete(); } );

		TaskPtr task_cb = new TaskFunction( provider );
		task_cb->setChain( this );

		task_last->addNext( task_cb );

		this->processTask( task_first, false );

		return true;
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
	void Chain::complete()
	{
		m_complete = true;

		if( m_cb != nullptr )
		{
			m_cb->call();
		}
	}
}