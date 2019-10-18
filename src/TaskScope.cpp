/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/TaskScope.h"
#include "GOAP/Source.h"
#include "GOAP/ScopeProvider.h"

#include "GOAP/Exception.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskScope::TaskScope( const ScopeProviderPtr & _provider )
        : m_provider( _provider )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskScope::~TaskScope()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskScope::_onRun( NodeInterface * _task )
    {
        SourcePtr source = _task->makeSource();

        m_provider->onScope( source );

        const SourceProviderInterfacePtr & provider = source->getSourceProvider();

        if( _task->injectSource( provider ) == false )
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
