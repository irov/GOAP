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
    typedef IntrusivePtr<class FunctionContextProviderInterface> FunctionContextProviderInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    class TaskFunctionContext
        : public TaskInterface
    {
        GOAP_DECLARE_VISITABLE( TaskInterface );

    public:
        explicit TaskFunctionContext( const FunctionContextProviderInterfacePtr & _provider );
        ~TaskFunctionContext() override;

    public:
        bool _onRun( NodeInterface * _node ) override;
        void _onFinally() override;

    protected:
        FunctionContextProviderInterfacePtr m_provider;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskFunctionContext> TaskFunctionContextPtr;
    //////////////////////////////////////////////////////////////////////////
}