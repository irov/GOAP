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
	Description & desc = m_schedulers[_id];

	desc.dead = true;

	desc.observer->onScheduleStop( _id );
}

struct Scheduler::PScheduleDead
{
	bool operator ()( const TMapSchedulers::value_type & _event ) const
	{
		return _event.second.dead;
	}
};

void Scheduler::update( float _time )
{
	for( TMapSchedulers::iterator
		it = m_schedulers.begin(),
		it_end = m_schedulers.end();
	it != it_end;
	++it )
	{
		Description & desc = it->second;
		
		if( desc.time < 0.f )
		{
			continue;
		}

		desc.time += _time;

		if( desc.loop == true )
		{
			while( desc.time >= desc.delay )
			{
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
	while( (it_erase = std::find_if( it_erase, m_schedulers.end(), PScheduleDead() )) != m_schedulers.end() )
		m_schedulers.erase( it_erase++ );
}