/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Array.h"
#include "GOAP/Transcriptor.h"
#include "GOAP/TaskRaceNeck.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
	typedef IntrusivePtr<class Source> SourcePtr;
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
			TaskPtr task_parallel_neck = GOAP_NEW TaskRaceNeck();
			task_parallel_neck->setChain( _chain );

			for( const SourcePtr & parallel_source : m_sources )
			{
				TaskPtr task = parallel_source->parse( _chain, _task );

				task->addNext( task_parallel_neck );
			}

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
			TranscriptorRaceArrayPtr<Count> transcriptor = GOAP_NEW TranscriptorRaceArray<Count>( _sources );

			return transcriptor;
		}
	}
}