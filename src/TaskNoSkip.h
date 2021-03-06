/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/TaskInterface.h"

namespace GOAP
{
    class TaskNoSkip
        : public TaskInterface
    {
        GOAP_DECLARE_VISITABLE( TaskInterface );

    public:
        TaskNoSkip( Allocator * _allocator );
        ~TaskNoSkip() override;

    protected:
        bool _onRun( NodeInterface * _node ) override;
        bool _onSkipable() const override;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskNoSkip> TaskNoSkipPtr;
    //////////////////////////////////////////////////////////////////////////
}