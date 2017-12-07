/*
* Copyright (C) 2017, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	pragma once

#	include "GOAP/Task.h"

namespace GOAP
{
    typedef IntrusivePtr<class GuardProvider> GuardProviderPtr;

    class TaskGuard
        : public Task
    {
    public:
        TaskGuard( const GuardProviderPtr & _begin, const GuardProviderPtr & _end );
        ~TaskGuard() override;

    public:
        bool _onRun() override;
        void _onFinally() override;

    public:
        bool _onSkipable() const override;

    protected:
        GuardProviderPtr m_begin;
        GuardProviderPtr m_end;
    };

    typedef IntrusivePtr<TaskGuard> TaskGuardPtr;
}