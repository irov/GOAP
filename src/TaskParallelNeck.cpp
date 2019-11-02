/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/TaskParallelNeck.h"

#include "GOAP/Node.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskParallelNeck::TaskParallelNeck()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskParallelNeck::~TaskParallelNeck()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskParallelNeck::_onCheckRun( const NodeInterface * _node ) const
    {
        bool result = _node->isEmptyPrevs();

        return result;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskParallelNeck::_onCheckSkip( const NodeInterface * _node ) const
    {
        bool result = _node->isEmptyPrevs();

        return result;
    }
}