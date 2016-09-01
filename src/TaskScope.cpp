#	include "GOAP/TaskScope.h"
#	include "GOAP/Source.h"
#	include "GOAP/ScopeProvider.h"

namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
	TaskScope::TaskScope( const ScopeProviderPtr & _provider )
		: m_provider(_provider)
	{
	}
	//////////////////////////////////////////////////////////////////////////
	TaskScope::~TaskScope()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	bool TaskScope::onRun()
	{
		GOAP::SourcePtr source = new GOAP::Source();

		bool skip = this->isSkip();
		source->setSkip( skip );

		if( m_provider->onScope( source ) == false )
		{
			return true;
		}

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
