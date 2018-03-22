/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/TaskBlock.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskBlock::TaskBlock()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskBlock::~TaskBlock()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskBlock::_onRun()
    {
        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskBlock::_onSkipable() const
    {
        return false;
    }
}