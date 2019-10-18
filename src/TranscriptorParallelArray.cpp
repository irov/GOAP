/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
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
        void generateParallelSource( const ViewSources & _sources, const ChainPtr & _chain, const NodePtr & _task, const NodePtr & _neck )
        {
            for( const SourcePtr & source : _sources )
            {
                const SourceProviderInterfacePtr & provider = source->getSourceProvider();

                NodePtr new_task = provider->parse( _chain, _task );

                new_task->addNext( _neck );
            }
        }
    }
}