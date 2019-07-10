/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/ArraySources.h"
#include "GOAP/ViewSources.h"
#include "GOAP/Transcriptor.h"
#include "GOAP/TaskRaceNeck.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    namespace Detail
    {
        void generateRaceSource( const ViewSources & _sources, const ChainPtr & _chain, const TaskPtr & _task, const TaskPtr & _neck );
    }
    //////////////////////////////////////////////////////////////////////////
    template<size_t Count>
    class TranscriptorRaceArray
        : public Transcriptor
    {
    public:
        TranscriptorRaceArray( const ArraySources<Count> & _sources )
            : m_sources( _sources )
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
        TaskPtr generate( const ChainPtr & _chain, const TaskPtr & _task ) override
        {
            TaskPtr task_parallel_neck = new TaskRaceNeck();
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
        TranscriptorRaceArrayPtr<Count> makeTranscriptorRaceArray( const ArraySources<Count> & _sources )
        {
            TranscriptorRaceArrayPtr<Count> transcriptor = new TranscriptorRaceArray<Count>( _sources );

            return transcriptor;
        }
    }
}