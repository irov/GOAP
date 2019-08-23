#include "Scheduler.h"

#include <algorithm>

Scheduler::Scheduler()
    : m_enumerator( 0 )
{
}

Scheduler::~Scheduler()
{
}

uint32_t Scheduler::schedule( float _delay, bool _loop, const SchedulerObserverPtr & _observer )
{
    uint32_t id = ++m_enumerator;

    Description desc;
    desc.id = id;
    desc.delay = _delay;
    desc.time = 0.f;
    desc.observer = _observer;
    desc.loop = _loop;
    desc.dead = false;

    m_schedulers.emplace_back( desc );

    return id;
}

void Scheduler::stop( uint32_t _id )
{
    VectorSchedulers::iterator it_found = std::find_if( m_schedulers.begin(), m_schedulers.end(), [_id]( const Description & _desc )
    {
        return _desc.id == _id;
    } );

    if( it_found == m_schedulers.end() )
    {
        return;
    }

    Description & desc = *it_found;

    desc.dead = true;

    desc.observer->onScheduleStop( _id );
    desc.observer = nullptr;
}

void Scheduler::update( float _time )
{
    for( Description & desc : m_schedulers )
    {
        if( desc.dead == true )
        {
            continue;
        }

        desc.time += _time;

        if( desc.loop == true )
        {
            while( desc.time >= desc.delay )
            {
                if( desc.dead == true )
                {
                    break;
                }

                desc.time -= desc.delay;

                uint32_t id = desc.id;

                desc.observer->onScheduleComplete( id );
            }
        }
        else
        {
            if( desc.time >= desc.delay )
            {
                desc.dead = true;

                uint32_t id = desc.id;

                desc.observer->onScheduleComplete( id );
            }
        }
    }

    VectorSchedulers::iterator it_erase = std::remove_if( m_schedulers.begin(), m_schedulers.end(), []( const Description & _event )
    {
        return _event.dead;
    } );

    m_schedulers.erase( it_erase, m_schedulers.end() );
}