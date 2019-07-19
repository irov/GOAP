/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/TaskParallelNeck.h"

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
    bool TaskParallelNeck::_onCheckRun() const
    {
        if( m_prevs.empty() == true )
        {
            return true;
        }

        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskParallelNeck::_onCheckSkip() const
    {
        if( m_prevs.empty() == true )
        {
            return true;
        }

        return false;
    }
}