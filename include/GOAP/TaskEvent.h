/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Task.h"
#include "GOAP/Event.h"

namespace GOAP
{
    class TaskEvent
        : public Task
    {
        GOAP_DECLARE_VISITABLE( TaskInterface );

    public:
        TaskEvent( const EventPtr & _event );
        ~TaskEvent() override;

    public:
        bool _onRun( NodeInterface * _node ) override;
        void _onFinally() override;
        bool _onSkipable() const override;

    protected:
        EventPtr m_event;
        EventProviderPtr m_provider;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskEvent> TaskEventPtr;
    //////////////////////////////////////////////////////////////////////////
}