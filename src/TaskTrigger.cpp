/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TaskTrigger.h"

#include "GOAP/EventInterface.h"
#include "GOAP/NodeInterface.h"
#include "GOAP/TriggerProviderInterface.h"
#include "GOAP/Cook.h"
#include "GOAP/SourceProviderInterface.h"
#include "GOAP/Exception.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskTrigger::TaskTrigger( const EventInterfacePtr & _event, const TriggerProviderInterfacePtr & _provider )
        : m_event( _event )
        , m_provider( _provider )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskTrigger::~TaskTrigger()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskTrigger::_onRun( NodeInterface * _node )
    {
        SourceInterfacePtr source = _node->makeSource();

        SourceInterfacePtr fork_source = Cook::addFork( source );

        bool result = m_provider->onTrigger( fork_source );

        if( result == false )
        {
            Cook::addEvent( source, m_event );

            Cook::addTriggerProvider( source, m_event, m_provider );
        }

        const SourceProviderInterfacePtr & provider = source->getSourceProvider();

        bool skip = _node->isSkip();
        provider->setSkip( skip );

        if( _node->injectSource( provider ) == false )
        {
            Helper::throw_exception( "TaskTrigger invalid fork source" );
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskTrigger::_onFinally()
    {
        m_event = nullptr;
        m_provider = nullptr;
    }
}
