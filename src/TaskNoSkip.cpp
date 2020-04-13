/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TaskNoSkip.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskNoSkip::TaskNoSkip()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskNoSkip::~TaskNoSkip()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskNoSkip::_onRun( NodeInterface * _node )
    {
        GOAP_UNUSED( _node );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskNoSkip::_onSkipable() const
    {
        return false;
    }
}