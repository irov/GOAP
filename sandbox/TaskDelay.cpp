#include "TaskDelay.h"

#include "GOAP/NodeInterface.h"

//////////////////////////////////////////////////////////////////////////
class TaskDelay::MySchedulerObserver
    : public SchedulerObserver
{
public:
    MySchedulerObserver( GOAP::Allocator * _allocator, GOAP::NodeInterface * _node )
        : SchedulerObserver( _allocator )
        , m_node( _node )
    {
    }

protected:
    void onScheduleComplete( uint32_t _id ) override
    {
        GOAP_UNUSED( _id );

        m_node->complete();
    }
    
    void onScheduleStop( uint32_t _id ) override
    {
        GOAP_UNUSED( _id );
    }

protected:
    GOAP::NodeInterfacePtr m_node;
};
//////////////////////////////////////////////////////////////////////////
TaskDelay::TaskDelay( GOAP::Allocator * _allocator, float _delay, const SchedulerPtr & _scheduler )
    : TaskInterface( _allocator )
    , m_delay( _delay )
    , m_scheduler( _scheduler )
    , m_id( 0 )
{
}
//////////////////////////////////////////////////////////////////////////
TaskDelay::~TaskDelay()
{
}
//////////////////////////////////////////////////////////////////////////
bool TaskDelay::_onRun( GOAP::NodeInterface * _node )
{
    typedef GOAP::IntrusivePtr<MySchedulerObserver> MySchedulerObserverPtr;

    GOAP::Allocator * allocator = this->getAllocator();

    MySchedulerObserver * observer = allocator->allocateT<MySchedulerObserver>( _node );

    m_id = m_scheduler->schedule( m_delay, false, MySchedulerObserverPtr::from( observer ) );

    return false;
}
//////////////////////////////////////////////////////////////////////////
void TaskDelay::_onSkip()
{
    m_scheduler->stop( m_id );
}
