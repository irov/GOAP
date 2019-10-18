#pragma once

#include "GOAP/Task.h"
#include "Scheduler.h"

class TaskDelay
    : public GOAP::Task
{
public:
    TaskDelay( float _delay, const SchedulerPtr & _scheduler );
    ~TaskDelay() override;

protected:
    bool _onRun( GOAP::NodeInterface * _task ) override;
    void _onSkip() override;

protected:
    float m_delay;

    SchedulerPtr m_scheduler;
    uint32_t m_id;

    class MySchedulerObserver;
};