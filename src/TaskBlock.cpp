/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TaskBlock.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskBlock::TaskBlock( Allocator * _allocator )
        : TaskInterface( _allocator )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskBlock::~TaskBlock()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskBlock::_onRun( NodeInterface * _node )
    {
        GOAP_UNUSED( _node );

        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskBlock::_onSkipable() const
    {
        return false;
    }
    //////////////////////////////////////////////////////////////////////////
}