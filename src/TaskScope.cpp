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
    bool TaskScope::_onRun()
    {
        SourcePtr source = Helper::makeSource();

        bool skip = this->isSkip();
        source->setSkip( skip );

        m_provider->onScope( source );

        if( this->injectSource( source ) == false )
        {
            Helper::throw_exception( "TaskScope invalid inject source" );
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskScope::_onFinally()
    {
        m_provider = nullptr;
    }
}
