/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TaskRaceNeck.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskRaceNeck::TaskRaceNeck()
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