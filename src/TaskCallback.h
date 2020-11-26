/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/TaskInterface.h"
#include "GOAP/CallbackProviderInterface.h"

namespace GOAP
{
    class TaskCallback
        : public TaskInterface
    {
        GOAP_DECLARE_VISITABLE( TaskInterface );

    public:
        TaskCallback( Allocator * _allocator, const CallbackProviderInterfacePtr & _provider );
        ~TaskCallback() override;

    public:
        bool _onRun( NodeInterface * _node ) override;
        void _onFinally() override;
        bool _onSkipable() const override;

    protected:
        CallbackProviderInterfacePtr m_provider;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskCallback> TaskCallbackPtr;
    //////////////////////////////////////////////////////////////////////////
}