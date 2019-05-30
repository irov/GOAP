/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/ArraySources.h"
#include "GOAP/Transcriptor.h"
#include "GOAP/TaskParallelNeck.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
	template<size_t Count>
	class TranscriptorParallelArray
		: public Transcriptor
	{
	public:
		TranscriptorParallelArray( const ArraySources<Count> & _sources )
			: m_sources( _sources )
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
		TaskPtr generate( const ChainPtr & _chain, const TaskPtr & _task ) override
		{
			TaskPtr task_parallel_neck = GOAP_NEW TaskParallelNeck();
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
	using TranscriptorParallelArrayPtr = IntrusivePtr<TranscriptorParallelArray<Count>>;
	//////////////////////////////////////////////////////////////////////////
	namespace Helper
	{
		template<size_t Count>
		TranscriptorParallelArrayPtr<Count> makeTranscriptorParallelArray( const ArraySources<Count> & _sources )
		{
			TranscriptorParallelArrayPtr<Count> transcriptor = GOAP_NEW TranscriptorParallelArray<Count>( _sources );

			return transcriptor;
		}
	}
}