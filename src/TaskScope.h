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
    typedef IntrusivePtr<class ScopeProviderInterface> ScopeProviderInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    class TaskScope
        : public TaskInterface
    {
        GOAP_DECLARE_VISITABLE( TaskInterface );

    public:
        TaskScope( Allocator * _allocator, const ScopeProviderInterfacePtr & _provider );
        ~TaskScope() override;

    public:
        bool _onRun( NodeInterface * _node ) override;
        void _onFinally() override;

    protected:
        ScopeProviderInterfacePtr m_provider;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskScope> TaskScopePtr;
    //////////////////////////////////////////////////////////////////////////
}