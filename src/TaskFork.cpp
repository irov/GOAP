#	include "GOAP/TaskFork.h"
#	include "GOAP/Source.h"
#	include "GOAP/Chain.h"
#	include "GOAP/ChainProvider.h"

namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
	TaskFork::TaskFork( const SourcePtr & _fork )
        : Task( TASK_EVENT_RUN )
		, m_fork(_fork)
	{
	}
	//////////////////////////////////////////////////////////////////////////
	TaskFork::~TaskFork()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	bool TaskFork::_onRun()
	{
		bool skip = this->isSkip();
		m_fork->setSkip( skip );

		ChainPtr chain = new Chain( m_fork );

		chain->run();

		m_chain = chain;

		return true;
	}
}
