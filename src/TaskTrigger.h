/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/TaskInterface.h"
#include "GOAP/EventInterface.h"
#include "GOAP/TriggerProviderInterface.h"

namespace GOAP
{
    class TaskTrigger
        : public TaskInterface
    {
        GOAP_DECLARE_VISITABLE( TaskInterface );

    public:
        TaskTrigger( Allocator * _allocator, const EventInterfacePtr & _event, const TriggerProviderInterfacePtr & _provider );
        ~TaskTrigger() override;

    public:
        bool _onRun( NodeInterface * _node ) override;
        void _onFinally() override;

    protected:
        EventInterfacePtr m_event;
        TriggerProviderInterfacePtr m_provider;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskTrigger> TaskTriggerPtr;
    //////////////////////////////////////////////////////////////////////////
}