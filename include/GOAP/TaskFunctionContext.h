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
    typedef IntrusivePtr<class FunctionContextProvider> FunctionContextProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    class TaskFunctionContext
        : public Task
    {
        GOAP_DECLARE_VISITABLE( Task );

    public:
        explicit TaskFunctionContext( const FunctionContextProviderPtr & _provider );
        ~TaskFunctionContext() override;

    public:
        bool _onRun() override;
        void _onFinally() override;

    protected:
        FunctionContextProviderPtr m_provider;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskFunctionContext> TaskFunctionContextPtr;
    //////////////////////////////////////////////////////////////////////////
}