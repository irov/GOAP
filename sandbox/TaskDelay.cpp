#	include "TaskDelay.h"

//////////////////////////////////////////////////////////////////////////
TaskDelay::TaskDelay( float _delay, Scheduler * _scheduler )
	: m_delay( _delay )
	, m_scheduler( _scheduler )
	, m_id( 0 )
{
}
//////////////////////////////////////////////////////////////////////////
TaskDelay::~TaskDelay()
{
}
//////////////////////////////////////////////////////////////////////////
bool TaskDelay::onRun()
{
	m_id = m_scheduler->schedule( m_delay, false, this );

	return false;
}
//////////////////////////////////////////////////////////////////////////
void TaskDelay::onSkip()
{
	m_scheduler->stop( m_id );
}
//////////////////////////////////////////////////////////////////////////
void TaskDelay::onScheduleComplete( uint32_t _id )
{
	this->complete();
}
//////////////////////////////////////////////////////////////////////////
void TaskDelay::onScheduleStop( uint32_t _id )
{
	//this->complete();
}