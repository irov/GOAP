#	include "GOAP/TaskFunction.h"

#	include "GOAP/FunctionProvider.h"

namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
	TaskFunction::TaskFunction( const FunctionProviderPtr & _provider )
		: m_provider( _provider )
	{
	}
	//////////////////////////////////////////////////////////////////////////
	TaskFunction::~TaskFunction()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	bool TaskFunction::onRun()
	{
		m_provider->call();

		return true;
	}	
}