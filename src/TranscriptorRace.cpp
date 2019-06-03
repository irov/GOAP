/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TranscriptorRace.h"

#include "GOAP/Source.h"
#include "GOAP/Task.h"
#include "GOAP/TaskRaceNeck.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
	TranscriptorRace::TranscriptorRace( const VectorSources & _sources )
		: m_sources( _sources )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TranscriptorRace::~TranscriptorRace()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    const VectorSources & TranscriptorRace::getSources() const
    {
        return m_sources;
    }
    //////////////////////////////////////////////////////////////////////////
    TaskPtr TranscriptorRace::generate( const ChainPtr & _chain, const TaskPtr & _task )
    {
        if( m_sources.empty() == true )
        {
            return _task;
        }

        TaskPtr task_parallel_neck = new TaskRaceNeck();
        task_parallel_neck->setChain( _chain );

        for( const SourcePtr & parallel_source : m_sources )
        {
            TaskPtr task = parallel_source->parse( _chain, _task );

            task->addNext( task_parallel_neck );
        }

        return task_parallel_neck;
    }
}