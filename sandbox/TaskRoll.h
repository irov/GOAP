#pragma once

#include "GOAP/Task.h"

#include "Scheduler.h"

class TaskRoll
    : public GOAP::Task
{
public:
    TaskRoll( float _delay, uint32_t _roll, uint32_t _max, const SchedulerPtr & _scheduler );
    ~TaskRoll() override;

protected:
    bool _onRun( GOAP::NodeInterface * _node ) override;
    void _onSkip() override;

protected:
    float m_delay;
    uint32_t m_roll;
    uint32_t m_max;

    SchedulerPtr m_scheduler;
    uint32_t m_id;

    class MySchedulerObserver;
};
