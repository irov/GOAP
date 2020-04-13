/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/ArraySources.h"
#include "GOAP/ViewSources.h"
#include "GOAP/TranscriptorInterface.h"
#include "GOAP/NodeInterface.h"
#include "GOAP/ChainInterface.h"
#include "GOAP/SourceInterface.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    namespace Detail
    {
        TaskInterfacePtr makeTaskParallelNeck( Allocator * _allocator );
        void generateParallelSource( const ViewSources<SourceInterface> & _sources, const ChainInterfacePtr & _chain, const NodeInterfacePtr & _task, const NodeInterfacePtr & _neck );
    }
    //////////////////////////////////////////////////////////////////////////
    template<size_t Count>
    class TranscriptorParallelArray
        : public TranscriptorInterface
    {
    public:
        TranscriptorParallelArray( Allocator * _allocator, ArraySources<Count> && _sources )
            : m_allocator( _allocator )
            , m_sources( std::forward<ArraySources<Count>>( _sources ) )
        {
        }

        ~TranscriptorParallelArray() override
        {
        }

    public:
        const ArraySources<Count> & getSources() const
        {
            return m_sources;
        }

    public:
        NodeInterfacePtr generate( const ChainInterfacePtr & _chain, const NodeInterfacePtr & _task ) override
        {
            const SourceInterfacePtr & source = _chain->getSource();

            TaskInterfacePtr provider_parallel_neck = Detail::makeTaskParallelNeck( m_allocator );

            NodeInterfacePtr task_parallel_neck = source->makeNode( provider_parallel_neck );

            task_parallel_neck->setChain( _chain );

            Detail::generateParallelSource( m_sources, _chain, _task, task_parallel_neck );

            return task_parallel_neck;
        }

    protected:
        Allocator * m_allocator;

        ArraySources<Count> m_sources;
    };
    //////////////////////////////////////////////////////////////////////////
    template<size_t Count>
    using TranscriptorParallelArrayPtr = IntrusivePtr<TranscriptorParallelArray<Count>>;
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        template<size_t Count>
        TranscriptorParallelArrayPtr<Count> makeTranscriptorParallelArray( Allocator * _allocator, ArraySources<Count> && _sources )
        {
            TranscriptorParallelArray<Count> * transcriptor = _allocator->allocateT<TranscriptorParallelArray<Count>>( _allocator, std::forward<ArraySources<Count>>( _sources ) );

            return TranscriptorParallelArrayPtr<Count>::from( transcriptor );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}