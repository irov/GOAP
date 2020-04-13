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
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class GuardProviderInterface> GuardProviderInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    class TaskGuard
        : public TaskInterface
    {
        GOAP_DECLARE_VISITABLE( TaskInterface );

    public:
        TaskGuard( const GuardProviderInterfacePtr & _begin, const GuardProviderInterfacePtr & _end );
        ~TaskGuard() override;

    public:
        bool _onRun( NodeInterface * _node ) override;
        void _onCancel() override;
        void _onFinally() override;

    public:
        bool _onSkipable() const override;

    protected:
        GuardProviderInterfacePtr m_begin;
        GuardProviderInterfacePtr m_end;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskGuard> TaskGuardPtr;
    //////////////////////////////////////////////////////////////////////////
}