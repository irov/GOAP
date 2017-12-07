/*
* Copyright (C) 2017, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	include "GOAP/TaskDeadLock.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskDeadLock::TaskDeadLock()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskDeadLock::~TaskDeadLock()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskDeadLock::_onRun()
    {
        return false;
    }
}