/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TranscriptorParallel.h"

#include "GOAP/SourceInterface.h"
#include "GOAP/ChainInterface.h"
#include "GOAP/NodeInterface.h"

#include "GOAP/AllocatorHelper.h"

#include "TaskParallelNeck.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TranscriptorParallel::TranscriptorParallel( Allocator * _allocator, VectorSources && _sources )
        : TranscriptorInterface( _allocator )
        , m_sources( std::forward<VectorSources>( _sources ) )
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
    NodeInterfacePtr TranscriptorParallel::generate( const ChainInterfacePtr & _chain, const NodeInterfacePtr & _task )
    {
        if( m_sources.empty() == true )
        {
            return _task;
        }

        Allocator * allocator = this->getAllocator();

        TaskInterfacePtr provider_parallel_neck = Helper::makeTask<TaskParallelNeck>( allocator );

        NodeInterfacePtr task_parallel_neck = Helper::makeNode( allocator, provider_parallel_neck );

        task_parallel_neck->setChain( _chain );

        for( const SourceInterfacePtr & parallel_source : m_sources )
        {
            const SourceProviderInterfacePtr & parallel_provider = parallel_source->getSourceProvider();

            NodeInterfacePtr task = parallel_provider->parse( _chain, _task );

            task->addNext( task_parallel_neck );
        }

        return task_parallel_neck;
    }
}