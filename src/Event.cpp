/*
* Copyright (C) 2017, Yuriy Levchenko <irov13@mail.ru>
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
    class Event::FEventFind
    {
    public:
        FEventFind( const EventProviderPtr & _provider )
            : m_provider( _provider )
        {
        }

    public:
        bool operator () ( const Event::ProviderDesc & _desc ) const
        {
            return _desc.provider == m_provider;
        }

    protected:
        const EventProviderPtr & m_provider;
    };
    //////////////////////////////////////////////////////////////////////////
    class Event::FEventDead
    {
    public:
        bool operator () ( const Event::ProviderDesc & _desc ) const
        {
            return _desc.dead;
        }
    };
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
    void Event::addObserver( const EventProviderPtr & _eventProvider )
    {
        ProviderDesc desc;
        desc.provider = _eventProvider;
        desc.dead = false;

        if( m_process == 0 )
        {
            m_providers.push_back( desc );
        }
        else
        {
            m_providersAdd.push_back( desc );
        }
    }
    //////////////////////////////////////////////////////////////////////////
    bool Event::removeObserver( const EventProviderPtr & _eventProvider )
    {
        VectorProviders::iterator it_found_add = std::find_if( m_providersAdd.begin(), m_providersAdd.end(), FEventFind( _eventProvider ) );

        if( it_found_add != m_providersAdd.end() )
        {
            m_providersAdd.erase( it_found_add );

            return true;
        }

        VectorProviders::iterator it_found = std::find_if( m_providers.begin(), m_providers.end(), FEventFind( _eventProvider ) );

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
            it_found->provider = nullptr;
            it_found->dead = true;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void Event::call()
    {
        ++m_process;

        for( VectorProviders::const_iterator
            it = m_providers.begin(),
            it_end = m_providers.end();
            it != it_end;
            ++it )
        {
            const ProviderDesc & desc = *it;

            if( desc.dead == true )
            {
                continue;
            }

            const EventProviderPtr & provider = desc.provider;

            bool remove = desc.provider->onEvent();

            if( remove == false )
            {
                continue;
            }

            if( desc.dead == true )
            {
                continue;
            }

            this->removeObserver( provider );
        }

        --m_process;

        if( m_process == 0 )
        {
            VectorProviders::iterator it_erase = std::remove_if( m_providers.begin(), m_providers.end(), FEventDead() );
            m_providers.erase( it_erase, m_providers.end() );

            m_providers.insert( m_providers.end(), m_providersAdd.begin(), m_providersAdd.end() );
            m_providersAdd.clear();
        }
    }
}