/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/TaskFunctionContext.h"

#include "GOAP/FunctionContextProvider.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskFunctionContext::TaskFunctionContext( const FunctionContextProviderPtr & _provider )
        : m_provider( _provider )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskFunctionContext::~TaskFunctionContext()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskFunctionContext::_onRun()
    {
        bool skip = this->isSkip();

        m_provider->onFunctionContext( skip );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskFunctionContext::_onFinally()
    {
        m_provider = nullptr;
    }
}