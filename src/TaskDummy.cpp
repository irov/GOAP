/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TaskDummy.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskDummy::TaskDummy( Allocator * _allocator )
        : TaskInterface( _allocator )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskDummy::~TaskDummy()
    {
    }
    //////////////////////////////////////////////////////////////////////////
}