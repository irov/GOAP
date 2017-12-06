/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/Event.h"
#include "GOAP/EventProvider.h"

#include <algorithm>

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    Event::Event()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Event::~Event()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    void Event::addObserver( const EventProviderPtr & _eventProvider )
    {
        m_eventProviders.push_back( _eventProvider );
    }
    //////////////////////////////////////////////////////////////////////////
    bool Event::removeObserver( const EventProviderPtr & _eventProvider )
    {
        VectorEventProvider::iterator it_found = std::find( m_eventProviders.begin(), m_eventProviders.end(), _eventProvider );

        if( it_found == m_eventProviders.end() )
        {
            return false;
        }

        m_eventProviders.erase( it_found );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void Event::call()
    {
        VectorEventProvider copy_eventProviders = m_eventProviders;

        for( const EventProviderPtr & event : copy_eventProviders )
        {
            bool remove = event->onEvent();

            if( remove == true )
            {
                this->removeObserver( event );
            }
        }
    }
}