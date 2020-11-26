/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "Timer.h"

#include "GOAP/TimerProviderInterface.h"

#include <algorithm>

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    Timer::Timer( Allocator * _allocator )
        : TimerInterface( _allocator )
        , m_providerAdd( StlAllocator<TimerDesc>( _allocator ) )
        , m_provider( StlAllocator<TimerDesc>( _allocator ) )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Timer::~Timer()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    void Timer::addTimerProvider( const TimerProviderInterfacePtr & _provider )
    {
        TimerDesc desc;

        desc.provider = _provider;
        desc.dead = false;

        m_providerAdd.push_back( desc );
    }
    //////////////////////////////////////////////////////////////////////////
    void Timer::removeTimerProvider( const TimerProviderInterfacePtr & _provider )
    {
        VectorTimerProvider::iterator it_found = std::find_if( m_provider.begin(), m_provider.end(), [&_provider]( const TimerDesc & _desc )
        {
            return _desc.provider == _provider;
        } );

        if( it_found != m_provider.end() )
        {
            TimerDesc & remove_desc = *it_found;

            remove_desc.dead = true;

            return;
        }

        VectorTimerProvider::iterator it_add_found = std::find_if( m_providerAdd.begin(), m_providerAdd.end(), [&_provider]( const TimerDesc & _desc )
        {
            return _desc.provider == _provider;
        } );

        if( it_add_found != m_providerAdd.end() )
        {
            *it_add_found = m_providerAdd.back();
            m_providerAdd.pop_back();

            return;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void Timer::update( float _time )
    {
        m_provider.insert( m_provider.end(), m_providerAdd.begin(), m_providerAdd.end() );
        m_providerAdd.clear();

        for( const TimerDesc & desc : m_provider )
        {
            if( desc.dead == true )
            {
                continue;
            }

            const TimerProviderInterfacePtr & provider = desc.provider;

            provider->onTime( _time );
        }

        m_provider.erase( std::remove_if( m_provider.begin(), m_provider.end(), []( const TimerDesc & _desc )
        {
            return _desc.dead;
        } ), m_provider.end() );
    }
    //////////////////////////////////////////////////////////////////////////
}
