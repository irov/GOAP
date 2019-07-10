/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TranscriptorParallel.h"

#include "GOAP/Source.h"
#include "GOAP/Task.h"
#include "GOAP/TaskParallelNeck.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TranscriptorParallel::TranscriptorParallel( const VectorSources & _sources )
        : m_sources( _sources )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TranscriptorParallel::~TranscriptorParallel()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    const VectorSources & TranscriptorParallel::getSources() const
    {
        return m_sources;
    }
    //////////////////////////////////////////////////////////////////////////
    TaskPtr TranscriptorParallel::generate( const ChainPtr & _chain, const TaskPtr & _task )
    {
        if( m_sources.empty() == true )
        {
            return _task;
        }

        TaskPtr task_parallel_neck = new TaskParallelNeck();
        task_parallel_neck->setChain( _chain );

        for( const SourcePtr & parallel_source : m_sources )
        {
            TaskPtr task = parallel_source->parse( _chain, _task );

            task->addNext( task_parallel_neck );
        }

        return task_parallel_neck;
    }
}