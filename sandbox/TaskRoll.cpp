#include "TaskRoll.h"

#include "GOAP/Node.h"

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////
class TaskRoll::MySchedulerObserver
    : public SchedulerObserver
{
public:
    MySchedulerObserver( GOAP::NodeInterface * _task, uint32_t _roll, uint32_t _max )
        : m_task( _task )
        , m_roll( _roll )
        , m_max( _max )
    {
    }

protected:
    void onScheduleComplete( uint32_t _id ) override
    {
        uint32_t roll = rand() % m_max;

        if( roll != m_roll )
        {
            return;
        }

        m_task->complete();
    }

    void onScheduleStop( uint32_t _id ) override
    {
    }

protected:
    GOAP::NodeInterfacePtr m_task;

    uint32_t m_roll;
    uint32_t m_max;
};
//////////////////////////////////////////////////////////////////////////
TaskRoll::TaskRoll( float _delay, uint32_t _roll, uint32_t _max, const SchedulerPtr & _scheduler )
    : m_delay( _delay )
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
bool TaskRoll::_onRun( GOAP::NodeInterface * _task )
{
    typedef GOAP::IntrusivePtr<MySchedulerObserver> MySchedulerObserverPtr;

    MySchedulerObserverPtr observer( new MySchedulerObserver( _task, m_roll, m_max ) );

    m_id = m_scheduler->schedule( m_delay, true, observer );

    return false;
}
//////////////////////////////////////////////////////////////////////////
void TaskRoll::_onSkip()
{
    m_scheduler->stop( m_id );
}
