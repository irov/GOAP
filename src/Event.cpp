/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "Event.h"
#include "GOAP/EventProviderInterface.h"

#include <algorithm>

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    Event::Event()
        : m_process( 0 )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Event::~Event()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    void Event::addProvider( const EventProviderInterfacePtr & _eventProvider )
    {
        ProviderDesc desc;
        desc.provider = _eventProvider;
        desc.dead = false;

        if( m_process == 0 )
        {
            m_providers.emplace_back( desc );
        }
        else
        {
            m_providersAdd.emplace_back( desc );
        }
    }
    //////////////////////////////////////////////////////////////////////////
    bool Event::removeProvider( const EventProviderInterfacePtr & _eventProvider )
    {
        VectorProviders::iterator it_found_add = std::find_if( m_providersAdd.begin(), m_providersAdd.end(), [&_eventProvider]( const Event::ProviderDesc & _desc )
        {
            return _desc.provider == _eventProvider;
        } );

        if( it_found_add != m_providersAdd.end() )
        {
            m_providersAdd.erase( it_found_add );

            return true;
        }

        VectorProviders::iterator it_found = std::find_if( m_providers.begin(), m_providers.end(), [&_eventProvider]( const Event::ProviderDesc & _desc )
        {
            return _desc.provider == _eventProvider;
        } );

        if( it_found == m_providers.end() )
        {
            return false;
        }

        if( m_process == 0 )
        {
            m_providers.erase( it_found );
        }
        else
        {
            ProviderDesc & desc = *it_found;

            desc.provider = nullptr;
            desc.dead = true;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void Event::clearProviders()
    {
        m_providersAdd.clear();

        if( m_process != 0 )
        {
            for( ProviderDesc & desc : m_providers )
            {
                desc.dead = true;
            }

            return;
        }

        m_providers.clear();
    }
    //////////////////////////////////////////////////////////////////////////
    void Event::call()
    {
        this->incref();

        ++m_process;

        for( const ProviderDesc & desc : m_providers )
        {
            if( desc.dead == true )
            {
                continue;
            }

            EventProviderInterfacePtr provider = desc.provider;

            bool remove = provider->onEvent();

            if( remove == false )
            {
                continue;
            }

            if( desc.dead == true )
            {
                continue;
            }

            this->removeProvider( provider );
        }

        --m_process;

        if( m_process == 0 )
        {
            m_providers.insert( m_providers.end(), m_providersAdd.begin(), m_providersAdd.end() );
            m_providersAdd.clear();

            VectorProviders::iterator it_erase = std::remove_if( m_providers.begin(), m_providers.end(), []( const Event::ProviderDesc & _desc )
            {
                return _desc.dead;
            } );

            m_providers.erase( it_erase, m_providers.end() );
        }

        this->decref();
    }
}