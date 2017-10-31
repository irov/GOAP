/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	include "TaskParallelNeck.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskParallelNeck::TaskParallelNeck()
        : Task( TASK_EVENT_CHECK_RUN | TASK_EVENT_CHECK_SKIP )
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