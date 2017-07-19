#	include "GOAP/TaskScope.h"
#	include "GOAP/Source.h"
#	include "GOAP/ScopeProvider.h"

namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
	TaskScope::TaskScope( const ScopeProviderPtr & _provider )
        : Task( TASK_EVENT_RUN )
		, m_provider(_provider)
	{
	}
	//////////////////////////////////////////////////////////////////////////
	TaskScope::~TaskScope()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	bool TaskScope::_onRun()
	{
		GOAP::SourcePtr source = new GOAP::Source();

		bool skip = this->isSkip();
		source->setSkip( skip );

		if( m_provider->onScope( source ) == false )
		{
			return true;
		}

        if( this->injectSource( source ) == false )
        {

        }

		return true;
	}
}
