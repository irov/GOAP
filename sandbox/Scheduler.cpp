#	include "Scheduler.h"

#	include <algorithm>

Scheduler::Scheduler()
	: m_enumerator( 0 )
{
}

Scheduler::~Scheduler()
{
}

uint32_t Scheduler::schedule( float _delay, bool _loop, SchedulerObserver * _observer )
{
	uint32_t id = ++m_enumerator;

	Description desc;
	desc.delay = _delay;
	desc.time = 0.f;
	desc.observer = _observer;
	desc.loop = _loop;
	desc.dead = false;
	
	m_schedulers[id] = desc;

	return id;
}

void Scheduler::stop( uint32_t _id )
{
	TMapSchedulers::iterator it_found = m_schedulers.find( _id );

	if( it_found == m_schedulers.end() )
	{
		return;
	}

	Description & desc = it_found->second;

	desc.dead = true;

	desc.observer->onScheduleStop( _id );
	desc.observer = nullptr;
}

void Scheduler::update( float _time )
{
	for( TMapSchedulers::iterator
		it = m_schedulers.begin(),
		it_end = m_schedulers.end();
	it != it_end;
	++it )
	{
		Description & desc = it->second;

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

				uint32_t id = it->first;

				desc.observer->onScheduleComplete( id );
			}
		}
		else
		{
			if( desc.time >= desc.delay )
			{
				desc.dead = true;

				uint32_t id = it->first;

				desc.observer->onScheduleComplete( id );
			}
		}
	}

	TMapSchedulers::iterator it_erase = m_schedulers.begin();
	while( (it_erase = std::find_if( it_erase, m_schedulers.end(), [] ( const TMapSchedulers::value_type & _event ) { return _event.second.dead; } )) != m_schedulers.end() )
		m_schedulers.erase( it_erase++ );
}