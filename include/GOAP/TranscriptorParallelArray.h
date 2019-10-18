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
#include "GOAP/TaskParallelNeck.h"
#include "GOAP/Chain.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    namespace Detail
    {
        void generateParallelSource( const ViewSources & _sources, const ChainPtr & _chain, const NodePtr & _task, const NodePtr & _neck );
    }
    //////////////////////////////////////////////////////////////////////////
    template<size_t Count>
    class TranscriptorParallelArray
        : public TranscriptorInterface
    {
    public:
        TranscriptorParallelArray( ArraySources<Count> && _sources )
            : m_sources( std::forward<ArraySources<Count> &&>( _sources ) )
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
        NodePtr generate( const ChainPtr & _chain, const NodePtr & _task ) override
        {
            const SourceInterfacePtr & source = _chain->getSource();

            TaskInterfacePtr provider_parallel_neck = Helper::makeTask<TaskParallelNeck>();

            NodePtr task_parallel_neck = source->makeNode( provider_parallel_neck );

            task_parallel_neck->setChain( _chain );

            Detail::generateParallelSource( m_sources, _chain, _task, task_parallel_neck );

            return task_parallel_neck;
        }

    protected:
        ArraySources<Count> m_sources;
    };
    //////////////////////////////////////////////////////////////////////////
    template<size_t Count>
    using TranscriptorParallelArrayPtr = IntrusivePtr<TranscriptorParallelArray<Count>>;
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        template<size_t Count>
        TranscriptorParallelArrayPtr<Count> makeTranscriptorParallelArray( ArraySources<Count> && _sources )
        {
            return TranscriptorParallelArrayPtr<Count>::from( new TranscriptorParallelArray<Count>( std::move( _sources ) ) );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}