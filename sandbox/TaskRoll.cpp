#include "TaskRoll.h"

#include "GOAP/NodeInterface.h"

#include <cstdlib>

//////////////////////////////////////////////////////////////////////////
class TaskRoll::MySchedulerObserver
    : public SchedulerObserver
{
public:
    MySchedulerObserver( GOAP::Allocator * _allocator, GOAP::NodeInterface * _node, uint32_t _roll, uint32_t _max )
        : SchedulerObserver( _allocator )
        , m_node( _node )
        , m_roll( _roll )
        , m_max( _max )
    {
    }

protected:
    void onScheduleComplete( uint32_t _id ) override
    {
        GOAP_UNUSED( _id );

        uint32_t roll = rand() % m_max;

        if( roll != m_roll )
        {
            return;
        }

        m_node->complete();
    }

    void onScheduleStop( uint32_t _id ) override
    {
        GOAP_UNUSED( _id );
    }

protected:
    GOAP::NodeInterfacePtr m_node;

    uint32_t m_roll;
    uint32_t m_max;
};
//////////////////////////////////////////////////////////////////////////
TaskRoll::TaskRoll( GOAP::Allocator * _allocator, float _delay, uint32_t _roll, uint32_t _max, const SchedulerPtr & _scheduler )
    : TaskInterface( _allocator )
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
bool TaskRoll::_onRun( GOAP::NodeInterface * _node )
{
    typedef GOAP::IntrusivePtr<MySchedulerObserver> MySchedulerObserverPtr;

    GOAP::Allocator * allocator = this->getAllocator();

    MySchedulerObserver * observer = allocator->allocateT<MySchedulerObserver>( _node, m_roll, m_max );

    m_id = m_scheduler->schedule( m_delay, true, MySchedulerObserverPtr::from( observer ) );

    return false;
}
//////////////////////////////////////////////////////////////////////////
void TaskRoll::_onSkip()
{
    m_scheduler->stop( m_id );
}
