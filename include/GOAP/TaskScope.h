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
    typedef IntrusivePtr<class ScopeProvider> ScopeProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    class TaskScope
        : public Task
    {
        DECLARE_VISITABLE( Task );

    public:
        explicit TaskScope( const ScopeProviderPtr & _provider );
        ~TaskScope() override;

    public:
        bool _onRun() override;
        void _onFinally() override;

    protected:
        ScopeProviderPtr m_provider;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskScope> TaskScopePtr;
    //////////////////////////////////////////////////////////////////////////
}