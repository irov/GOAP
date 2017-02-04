#	include "GOAP/TaskSwitch.h"
#	include "GOAP/Source.h"
#	include "GOAP/SwitchProvider.h"

namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
	TaskSwitch::TaskSwitch( const SwitchProviderPtr & _provider, const TVectorSources & _sources )
        : Task( TASK_EVENT_RUN )
        , m_provider( _provider )
		, m_sources( _sources )
	{		
	}
	//////////////////////////////////////////////////////////////////////////
	TaskSwitch::~TaskSwitch()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	TVectorSources & TaskSwitch::getSources()
	{
		return m_sources;
	}
	//////////////////////////////////////////////////////////////////////////
	bool TaskSwitch::_onRun()
	{
		uint32_t id = m_provider->onSwitch();

		const SourcePtr & source = m_sources[id];

		bool skip = this->isSkip();
		source->setSkip( skip );

		TVectorTasks nexts;
		this->popNexts( nexts );

		const ChainPtr & chain = this->getChain();

		TaskPtr task = source->parse( chain, this );
		
		if( task == nullptr )
		{
			//TODO - Error

			return true;
		}

		for( TVectorTasks::iterator
			it = nexts.begin(),
			it_end = nexts.end();
		it != it_end;
		++it )
		{
			const TaskPtr & next = *it;

			task->addNext( next );
		}

		return true;
	}
}
