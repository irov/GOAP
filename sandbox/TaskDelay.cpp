#include "TaskDelay.h"

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
bool TaskDelay::_onRun()
{
    m_id = m_scheduler->schedule( m_delay, false, this );

    return false;
}
//////////////////////////////////////////////////////////////////////////
void TaskDelay::_onSkip()
{
    m_scheduler->stop( m_id );
}
//////////////////////////////////////////////////////////////////////////
void TaskDelay::onScheduleComplete( uint32_t _id )
{
    m_id = 0;

    this->complete();
}
//////////////////////////////////////////////////////////////////////////
void TaskDelay::onScheduleStop( uint32_t _id )
{
    m_id = 0;
}