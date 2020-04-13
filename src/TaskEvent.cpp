/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TaskEvent.h"

#include "GOAP/NodeInterface.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskEvent::TaskEvent( Allocator * _allocator, const EventInterfacePtr & _event )
        : m_allocator( _allocator )
        , m_event( _event )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskEvent::~TaskEvent()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskEvent::_onRun( NodeInterface * _node )
    {
        EventProviderInterfacePtr provider = Helper::makeEventProvider( m_allocator, [_node]()
        {
            _node->complete();

            return true;
        } );

        m_event->addProvider( provider );

        m_provider = provider;

        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskEvent::_onFinally()
    {
        if( m_provider != nullptr )
        {
            m_event->removeProvider( m_provider );
            m_provider = nullptr;
        }

        m_event = nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskEvent::_onSkipable() const
    {
        return false;
    }    
}