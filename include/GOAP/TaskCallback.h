/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Task.h"

#include "GOAP/CallbackProvider.h"

namespace GOAP
{
    class TaskCallback
        : public Task
    {
        GOAP_DECLARE_VISITABLE( Task );

    public:
        explicit TaskCallback( Allocator * _allocator, const CallbackProviderPtr & _provider );
        ~TaskCallback() override;

    public:
        bool _onRun( NodeInterface * _node ) override;
        void _onFinally() override;
        bool _onSkipable() const override;

    protected:
        Allocator * m_allocator;

        CallbackProviderPtr m_provider;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskCallback> TaskCallbackPtr;
    //////////////////////////////////////////////////////////////////////////
}