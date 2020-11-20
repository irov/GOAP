/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TaskSwitch.h"

#include "GOAP/SourceInterface.h"
#include "GOAP/SwitchProviderInterface.h"

#include "GOAP/Exception.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskSwitch::TaskSwitch( const SwitchProviderInterfacePtr & _provider, VectorSources && _sources )
        : m_provider( _provider )
        , m_sources( std::forward<VectorSources>( _sources ) )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskSwitch::~TaskSwitch()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    const VectorSources & TaskSwitch::getSources() const
    {
        return m_sources;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskSwitch::_onRun( NodeInterface * _node )
    {
        uint32_t id = m_provider->onSwitch();

        const SourceInterfacePtr & source = m_sources[id];

        const SourceProviderInterfacePtr & provider = source->getSourceProvider();

        bool skip = _node->isSkip();
        provider->setSkip( skip );

        if( _node->injectSource( provider ) == false )
        {
            Helper::throw_exception( "TaskSwitch invalid inject source" );
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskSwitch::_onFinally()
    {
        m_provider = nullptr;

        m_sources.clear();
    }
}
