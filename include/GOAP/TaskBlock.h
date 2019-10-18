/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Task.h"

namespace GOAP
{
    class TaskBlock
        : public Task
    {
        GOAP_DECLARE_VISITABLE( Task );

    public:
        TaskBlock();
        ~TaskBlock() override;

    protected:
        bool _onRun( NodeInterface * _task ) override;
        bool _onSkipable() const override;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskBlock> TaskBlockPtr;
    //////////////////////////////////////////////////////////////////////////
}