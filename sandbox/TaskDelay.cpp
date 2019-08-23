#include "TaskDelay.h"

//////////////////////////////////////////////////////////////////////////
class TaskDelay::MySchedulerObserver
    : public SchedulerObserver
{
public:
    MySchedulerObserver( TaskDelay * _task )
        : m_task( _task )
    {
    }

protected:
    void onScheduleComplete( uint32_t _id ) override
    {
        m_task->complete();
    }
    
    void onScheduleStop( uint32_t _id ) override
    {
    }

protected:
    TaskDelay * m_task;
};
//////////////////////////////////////////////////////////////////////////
TaskDelay::TaskDelay( float _delay, const SchedulerPtr & _scheduler )
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
    typedef GOAP::IntrusivePtr<MySchedulerObserver> MySchedulerObserverPtr;

    MySchedulerObserverPtr observer( new MySchedulerObserver( this ) );

    m_id = m_scheduler->schedule( m_delay, false, observer );

    return false;
}
//////////////////////////////////////////////////////////////////////////
void TaskDelay::_onSkip()
{
    m_scheduler->stop( m_id );
}
