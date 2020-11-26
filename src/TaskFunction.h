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
    typedef IntrusivePtr<class FunctionProviderInterface> FunctionProviderInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    class TaskFunction
        : public TaskInterface
    {
        GOAP_DECLARE_VISITABLE( TaskInterface );

    public:
        TaskFunction( Allocator * _allocator, const FunctionProviderInterfacePtr & _provider );
        ~TaskFunction() override;

    public:
        bool _onRun( NodeInterface * _node ) override;
        void _onFinally() override;

    protected:
        FunctionProviderInterfacePtr m_provider;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskFunction> TaskFunctionPtr;
    //////////////////////////////////////////////////////////////////////////
}