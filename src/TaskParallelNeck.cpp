#	include "TaskParallelNeck.h"

namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
	TaskParallelNeck::TaskParallelNeck()
        : Task( TASK_EVENT_CHECK_RUN | TASK_EVENT_CHECK_SKIP )
	{
	}
	//////////////////////////////////////////////////////////////////////////
	TaskParallelNeck::~TaskParallelNeck()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	bool TaskParallelNeck::_onCheckRun() const
	{
		if( m_prevs.empty() == true )
		{
			return true;
		}

		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	bool TaskParallelNeck::_onCheckSkip() const
	{
		if( m_prevs.empty() == true )
		{
			return true;
		}

		return false;
	}
}