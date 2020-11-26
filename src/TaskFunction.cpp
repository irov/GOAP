/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TaskFunction.h"

#include "GOAP/FunctionProviderInterface.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskFunction::TaskFunction( Allocator * _allocator, const FunctionProviderInterfacePtr & _provider )
        : TaskInterface(_allocator)
        , m_provider( _provider )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskFunction::~TaskFunction()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskFunction::_onRun( NodeInterface * _node )
    {
        GOAP_UNUSED( _node );

        FunctionProviderInterfacePtr provider = std::move( m_provider );
        provider->onFunction();

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskFunction::_onFinally()
    {
        m_provider = nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
}