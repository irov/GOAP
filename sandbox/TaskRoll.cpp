#	include "TaskRoll.h"

#	include <stdlib.h>

//////////////////////////////////////////////////////////////////////////
TaskRoll::TaskRoll( float _delay, uint32_t _roll, uint32_t _max, Scheduler * _scheduler )
    : GOAP::Task( GOAP::TASK_EVENT_RUN | GOAP::TASK_EVENT_SKIP )
	, m_delay( _delay )
	, m_roll( _roll )
	, m_max( _max )
	, m_scheduler( _scheduler )
	, m_id( 0 )
{
}
//////////////////////////////////////////////////////////////////////////
TaskRoll::~TaskRoll()
{
}
//////////////////////////////////////////////////////////////////////////
bool TaskRoll::_onRun()
{
	m_id = m_scheduler->schedule( m_delay, true, this );

	return false;
}
//////////////////////////////////////////////////////////////////////////
void TaskRoll::_onSkip()
{
	m_scheduler->stop( m_id );
}
//////////////////////////////////////////////////////////////////////////
void TaskRoll::onScheduleComplete( uint32_t _id )
{
	uint32_t roll = rand() % m_max;

	if( roll != m_roll )
	{
		return;
	}

	this->complete();
}
//////////////////////////////////////////////////////////////////////////
void TaskRoll::onScheduleStop( uint32_t _id )
{
	m_id = 0;
	m_scheduler = nullptr;
	//this->complete();
}
