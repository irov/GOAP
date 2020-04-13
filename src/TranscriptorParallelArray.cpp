/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/TranscriptorParallelArray.h"
#include "GOAP/SourceInterface.h"
#include "GOAP/SourceProviderInterface.h"
#include "GOAP/NodeInterface.h"

#include "TaskParallelNeck.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    namespace Detail
    {
        //////////////////////////////////////////////////////////////////////////
        TaskInterfacePtr makeTaskParallelNeck( Allocator * _allocator )
        {
            TaskInterfacePtr parallel_neck = Helper::makeTask<TaskParallelNeck>( _allocator );

            return parallel_neck;
        }
        //////////////////////////////////////////////////////////////////////////
        void generateParallelSource( const ViewSources<SourceInterface> & _sources, const ChainInterfacePtr & _chain, const NodeInterfacePtr & _task, const NodeInterfacePtr & _neck )
        {
            for( const SourceInterfacePtr & source : _sources )
            {
                const SourceProviderInterfacePtr & provider = source->getSourceProvider();

                NodeInterfacePtr new_task = provider->parse( _chain, _task );

                new_task->addNext( _neck );
            }
        }
    }
}