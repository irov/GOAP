/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TranscriptorRace.h"

#include "GOAP/Source.h"
#include "GOAP/Node.h"
#include "GOAP/TaskRaceNeck.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TranscriptorRace::TranscriptorRace( VectorSources && _sources )
        : m_sources( std::forward<VectorSources &&>( _sources ) )
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
    NodePtr TranscriptorRace::generate( const ChainPtr & _chain, const NodePtr & _task )
    {
        if( m_sources.empty() == true )
        {
            return _task;
        }

        const SourceInterfacePtr & source = _chain->getSource();

        TaskInterfacePtr provider_parallel_neck = Helper::makeTask<TaskRaceNeck>();

        NodePtr task_parallel_neck = source->makeNode( provider_parallel_neck );
        task_parallel_neck->setChain( _chain );

        for( const SourceInterfacePtr & race_source : m_sources )
        {
            const SourceProviderInterfacePtr & race_provider = race_source->getSourceProvider();

            NodePtr task = race_provider->parse( _chain, _task );

            task->addNext( task_parallel_neck );
        }

        return task_parallel_neck;
    }
}