/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TaskRaceNeck.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskRaceNeck::TaskRaceNeck( Allocator * _allocator )
        : TaskInterface( _allocator )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskRaceNeck::~TaskRaceNeck()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskRaceNeck::_onCheckRun( const NodeInterface * _node ) const
    {
        GOAP_UNUSED( _node );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskRaceNeck::_onCheckSkip( const NodeInterface * _node ) const
    {
        GOAP_UNUSED( _node );

        return true;
    }
}