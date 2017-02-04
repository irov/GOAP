#	include "GOAP/TaskIf.h"
#	include "GOAP/Source.h"
#	include "GOAP/IfProvider.h"

namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
	TaskIf::TaskIf( const IfProviderPtr & _provider, const SourcePtr & _sourceTrue, const SourcePtr & _sourceFalse )
        : Task( TASK_EVENT_RUN )
        , m_provider( _provider )
		, m_sourceTrue( _sourceTrue )
		, m_sourceFalse( _sourceFalse )
	{
	}
	//////////////////////////////////////////////////////////////////////////
	TaskIf::~TaskIf()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	bool TaskIf::_onRun()
	{
		bool result = m_provider->onIf();

		SourcePtr result_source;

		if( result == true )
		{
			result_source = m_sourceTrue;
		}
		else
		{
			result_source = m_sourceFalse;
		}

		bool skip = this->isSkip();
		result_source->setSkip( skip );

		TVectorTasks nexts;
		this->popNexts( nexts );

		const ChainPtr & chain = this->getChain();

		TaskPtr task = result_source->parse( chain, this );
		
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
