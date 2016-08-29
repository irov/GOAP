#	include "TranscriptorBase.h"

#	include "GOAP/Task.h"

namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
	TranscriptorBase::TranscriptorBase( const TaskPtr & _task )
		: m_task( _task )
	{
	}
	//////////////////////////////////////////////////////////////////////////
	TaskPtr TranscriptorBase::generate( const ChainPtr & _chain, const TaskPtr & _task )
	{
		(void)_chain;

		if( _task != nullptr )
		{
			m_task->setChain( _chain );

			_task->addNext( m_task );
		}

		return m_task;
	}
}