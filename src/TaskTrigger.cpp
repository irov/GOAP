/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/TaskTrigger.h"
#include "GOAP/Event.h"
#include "GOAP/EventProvider.h"
#include "GOAP/NodeInterface.h"
#include "GOAP/TriggerProvider.h"
#include "GOAP/Source.h"
#include "GOAP/SourceProviderInterface.h"
#include "GOAP/Exception.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskTrigger::TaskTrigger( const EventPtr & _event, const TriggerProviderPtr & _provider )
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
        SourcePtr source = _node->makeSource();

        SourcePtr fork_source = source->addFork();

        bool result = m_provider->onTrigger( fork_source );

        if( result == false )
        {
            source->addEvent( m_event );

            source->addTriggerProvider( m_event, m_provider );
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
