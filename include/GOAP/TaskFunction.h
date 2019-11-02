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
    typedef IntrusivePtr<class FunctionProvider> FunctionProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    class TaskFunction
        : public Task
    {
        GOAP_DECLARE_VISITABLE( TaskInterface );

    public:
        explicit TaskFunction( const FunctionProviderPtr & _provider );
        ~TaskFunction() override;

    public:
        bool _onRun( NodeInterface * _node ) override;
        void _onFinally() override;

    protected:
        FunctionProviderPtr m_provider;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskFunction> TaskFunctionPtr;
    //////////////////////////////////////////////////////////////////////////
}