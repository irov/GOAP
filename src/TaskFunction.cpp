#	include "GOAP/TaskFunction.h"

#	include "GOAP/FunctionProvider.h"

namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
	TaskFunction::TaskFunction( const FunctionProviderPtr & _provider )
        : Task( TASK_EVENT_RUN )
		, m_provider( _provider )
	{
	}
	//////////////////////////////////////////////////////////////////////////
	TaskFunction::~TaskFunction()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	bool TaskFunction::_onRun()
	{
		if( m_provider != nullptr )
		{
			m_provider->onFunction();
		}

		return true;
	}	
}