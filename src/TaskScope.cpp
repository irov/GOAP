/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TaskScope.h"

#include "GOAP/SourceInterface.h"
#include "GOAP/ScopeProviderInterface.h"

#include "GOAP/Exception.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskScope::TaskScope( Allocator * _allocator, const ScopeProviderInterfacePtr & _provider )
        : TaskInterface( _allocator )
        , m_provider( _provider )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskScope::~TaskScope()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskScope::_onRun( NodeInterface * _node )
    {
        SourceInterfacePtr source = _node->makeSource();

        m_provider->onScope( source );

        const SourceProviderInterfacePtr & provider = source->getSourceProvider();

        if( _node->injectSource( provider ) == false )
        {
            Helper::throw_exception( "TaskScope invalid inject source" );
        }

        m_provider = nullptr;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskScope::_onFinally()
    {
        m_provider = nullptr;
    }
}
