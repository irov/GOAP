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
#include "GOAP/IfProvider.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskTrigger::TaskTrigger( const EventPtr & _event, const IfProviderPtr & _provider )
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
        bool result = m_provider->onIf();

        if( result == true )
        {
            return true;
        }

        m_eventProvider = Helper::makeEventProvider( [this, _node]()
        {
            bool result = m_provider->onIf();

            if( result == true )
            {
                if( _node->isSkip() == false )
                {
                    _node->complete();
                }
                else
                {
                    _node->skip();
                }
            }

            return result;
        } );

        m_event->addProvider( m_eventProvider );

        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskTrigger::_onFinally()
    {
        if( m_event != nullptr )
        {
            if( m_eventProvider != nullptr )
            {
                m_event->removeProvider( m_eventProvider );
                m_eventProvider = nullptr;
            }

            m_event = nullptr;
        }

        m_provider = nullptr;
    }
}
