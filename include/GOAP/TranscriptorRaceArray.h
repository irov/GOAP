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
#include "GOAP/TaskRaceNeck.h"
#include "GOAP/Node.h"
#include "GOAP/Chain.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    namespace Detail
    {
        void generateRaceSource( const ViewSources<SourceInterface> & _sources, const ChainPtr & _chain, const NodePtr & _task, const NodePtr & _neck );
    }
    //////////////////////////////////////////////////////////////////////////
    template<size_t Count>
    class TranscriptorRaceArray
        : public TranscriptorInterface
    {
    public:
        TranscriptorRaceArray( ArraySources<Count> && _sources )
            : m_sources( std::forward<ArraySources<Count> &&>( _sources ) )
        {
        }

        ~TranscriptorRaceArray() override
        {
        }

    public:
        const ArraySources<Count> & getSources() const
        {
            return m_sources;
        }

    public:
        NodeInterfacePtr generate( const ChainPtr & _chain, const NodeInterfacePtr & _task ) override
        {
            if( m_sources.empty() == true )
            {
                return _task;
            }

            const SourceInterfacePtr & source = _chain->getSource();

            TaskInterfacePtr provider_parallel_neck = Helper::makeTask<TaskRaceNeck>();

            NodeInterfacePtr task_parallel_neck = source->makeNode( provider_parallel_neck );

            task_parallel_neck->setChain( _chain );

            Detail::generateRaceSource( m_sources, _chain, _task, task_parallel_neck );

            return task_parallel_neck;
        }

    protected:
        ArraySources<Count> m_sources;
    };
    //////////////////////////////////////////////////////////////////////////
    template<size_t Count>
    using TranscriptorRaceArrayPtr = IntrusivePtr<TranscriptorRaceArray<Count>>;
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        template<size_t Count>
        TranscriptorRaceArrayPtr<Count> makeTranscriptorRaceArray( ArraySources<Count> && _sources )
        {
            return TranscriptorRaceArrayPtr<Count>::from( new TranscriptorRaceArray<Count>( std::forward<ArraySources<Count> &&>( _sources ) ) );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}