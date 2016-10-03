#	include "TranscriptorFork.h"

#	include "GOAP/Source.h"
#	include "GOAP/TaskFork.h"

namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
	TranscriptorFork::TranscriptorFork()
	{
		m_source = new Source();
	}
	//////////////////////////////////////////////////////////////////////////
	const SourcePtr & TranscriptorFork::getSource() const
	{
		return m_source;
	}
	//////////////////////////////////////////////////////////////////////////
	TaskPtr TranscriptorFork::generate( const ChainPtr & _chain, const TaskPtr & _task )
	{
		TaskPtr task_fork = new TaskFork( m_source );
		task_fork->setChain( _chain );

		return task_fork;
	}
}