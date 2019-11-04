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
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class Event> EventPtr;
    typedef IntrusivePtr<class EventProvider> EventProviderPtr;
    typedef IntrusivePtr<class TriggerProvider> TriggerProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    class TaskTrigger
        : public Task
    {
        GOAP_DECLARE_VISITABLE( Task );

    public:
        TaskTrigger( const EventPtr & _event, const TriggerProviderPtr & _provider );
        ~TaskTrigger() override;

    public:
        bool _onRun( NodeInterface * _node ) override;
        void _onFinally() override;

    protected:
        EventPtr m_event;
        EventProviderPtr m_eventProvider;
        TriggerProviderPtr m_provider;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskTrigger> TaskTriggerPtr;
    //////////////////////////////////////////////////////////////////////////
}