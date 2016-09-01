#	include "TranscriptorRace.h"

#	include "TaskRaceNeck.h"

#	include "GOAP/Source.h"
#	include "GOAP/Task.h"


namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
	TranscriptorRace::TranscriptorRace( size_t _count )
	{
		m_sources.resize( _count );

		for( TVectorSources::iterator
			it = m_sources.begin(),
			it_end = m_sources.end();
		it != it_end;
		++it )
		{
			*it = new Source();
		}
	}
	//////////////////////////////////////////////////////////////////////////
	TVectorSources & TranscriptorRace::getSources()
	{
		return m_sources;
	}
	//////////////////////////////////////////////////////////////////////////
	TaskPtr TranscriptorRace::generate( const ChainPtr & _chain, const TaskPtr & _task )
	{
		TaskPtr task_parallel_neck = new TaskRaceNeck();
		task_parallel_neck->setChain( _chain );

		for( TVectorSources::iterator
			it = m_sources.begin(),
			it_end = m_sources.end();
		it != it_end;
		++it )
		{
			const SourcePtr & parallel_source = *it;

			TaskPtr task = parallel_source->parse( _chain, _task );

			task->addNext( task_parallel_neck );
		}

		return task_parallel_neck;
	}
}