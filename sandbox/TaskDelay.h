#pragma once

#include "GOAP/TaskInterface.h"

#include "Scheduler.h"

class TaskDelay
    : public GOAP::TaskInterface
{
public:
    TaskDelay( GOAP::Allocator * _allocator, float _delay, const SchedulerPtr & _scheduler );
    ~TaskDelay() override;

protected:
    bool _onRun( GOAP::NodeInterface * _node ) override;
    void _onSkip() override;

protected:
    GOAP::Allocator * m_allocator;

    float m_delay;

    SchedulerPtr m_scheduler;
    uint32_t m_id;

    class MySchedulerObserver;
};