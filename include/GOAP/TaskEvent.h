/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Task.h"
#include "GOAP/EventInterface.h"

namespace GOAP
{
    class TaskEvent
        : public Task
    {
        GOAP_DECLARE_VISITABLE( TaskInterface );

    public:
        TaskEvent( Allocator * _allocator, const EventInterfacePtr & _event );
        ~TaskEvent() override;

    public:
        bool _onRun( NodeInterface * _node ) override;
        void _onFinally() override;
        bool _onSkipable() const override;

    protected:
        Allocator * m_allocator;

        EventInterfacePtr m_event;
        EventProviderInterfacePtr m_provider;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskEvent> TaskEventPtr;
    //////////////////////////////////////////////////////////////////////////
}