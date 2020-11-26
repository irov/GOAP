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
    class TaskBlock
        : public TaskInterface
    {
        GOAP_DECLARE_VISITABLE( TaskInterface );

    public:
        TaskBlock( Allocator * _allocator );
        ~TaskBlock() override;

    protected:
        bool _onRun( NodeInterface * _node ) override;
        bool _onSkipable() const override;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskBlock> TaskBlockPtr;
    //////////////////////////////////////////////////////////////////////////
}