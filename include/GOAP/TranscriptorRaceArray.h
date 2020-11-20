/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/ArraySources.h"
#include "GOAP/ViewSources.h"
#include "GOAP/KernelInterface.h"
#include "GOAP/TaskInterface.h"
#include "GOAP/TranscriptorInterface.h"
#include "GOAP/NodeInterface.h"
#include "GOAP/SourceInterface.h"
#include "GOAP/ChainInterface.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    namespace Detail
    {
        TaskInterfacePtr makeTaskRaceNeck( Allocator * _allocator );
        void generateRaceSource( const ViewSources<SourceInterface> & _sources, const ChainInterfacePtr & _chain, const NodeInterfacePtr & _task, const NodeInterfacePtr & _neck );
    }
    //////////////////////////////////////////////////////////////////////////
    template<size_t Count>
    class TranscriptorRaceArray
        : public TranscriptorInterface
    {
    public:
        TranscriptorRaceArray( ArraySources<Count> && _sources )
            : m_sources( std::forward<ArraySources<Count>>( _sources ) )
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
        NodeInterfacePtr generate( const ChainInterfacePtr & _chain, const NodeInterfacePtr & _task ) override
        {
            if( m_sources.empty() == true )
            {
                return _task;
            }

            KernelInterface * kernel = _chain->getKernel();

            Allocator * allocator = this->getAllocator();

            TaskInterfacePtr provider_parallel_neck = Detail::makeTaskRaceNeck( allocator );

            NodeInterfacePtr task_parallel_neck = kernel->makeNode( provider_parallel_neck );

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
        TranscriptorRaceArrayPtr<Count> makeTranscriptorRaceArray( Allocator * _allocator, ArraySources<Count> && _sources )
        {
            TranscriptorRaceArray<Count> * transcriptor = _allocator->allocateT<TranscriptorRaceArray<Count>>( std::forward<ArraySources<Count>>( _sources ) );

            return TranscriptorRaceArrayPtr<Count>::from( transcriptor );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}