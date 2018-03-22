/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/TaskSwitch.h"
#include "GOAP/Source.h"
#include "GOAP/SwitchProvider.h"

#include "GOAP/Exception.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskSwitch::TaskSwitch( const SwitchProviderPtr & _provider, const VectorSources & _sources )
        : m_provider( _provider )
        , m_sources( _sources )
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
    bool TaskSwitch::_onRun()
    {
        uint32_t id = m_provider->onSwitch();

        const SourcePtr & source = m_sources[id];

        bool skip = this->isSkip();
        source->setSkip( skip );

        if( this->injectSource( source ) == false )
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
