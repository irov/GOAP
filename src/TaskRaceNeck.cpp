/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	include "TaskRaceNeck.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskRaceNeck::TaskRaceNeck()
        : Task( TASK_EVENT_CHECK_RUN | TASK_EVENT_CHECK_SKIP )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskRaceNeck::~TaskRaceNeck()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskRaceNeck::_onCheckRun() const
    {
        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskRaceNeck::_onCheckSkip() const
    {
        return true;
    }
}