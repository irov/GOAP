/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/TranscriptorParallelArray.h"
#include "GOAP/Source.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    namespace Detail
    {
        void generateParallelSource( const ViewSources & _sources, const ChainPtr & _chain, const TaskPtr & _task, const TaskPtr & _neck )
        {
            for( const SourcePtr & parallel_source : _sources )
            {
                TaskPtr new_task = parallel_source->parse( _chain, _task );

                new_task->addNext( _neck );
            }
        }
    }
}