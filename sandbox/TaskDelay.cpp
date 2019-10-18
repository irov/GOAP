#include "TaskDelay.h"

#include "GOAP/Node.h"

//////////////////////////////////////////////////////////////////////////
class TaskDelay::MySchedulerObserver
    : public SchedulerObserver
{
public:
    MySchedulerObserver( GOAP::NodeInterface * _task )
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
    GOAP::NodeInterfacePtr m_task;
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
bool TaskDelay::_onRun( GOAP::NodeInterface * _task )
{
    typedef GOAP::IntrusivePtr<MySchedulerObserver> MySchedulerObserverPtr;

    MySchedulerObserverPtr observer( new MySchedulerObserver( _task ) );

    m_id = m_scheduler->schedule( m_delay, false, observer );

    return false;
}
//////////////////////////////////////////////////////////////////////////
void TaskDelay::_onSkip()
{
    m_scheduler->stop( m_id );
}
