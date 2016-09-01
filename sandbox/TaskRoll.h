#	pragma once

#	include "GOAP/Task.h"

#	include "Scheduler.h"

class TaskRoll
	: public GOAP::Task
	, public SchedulerObserver
{
public:
	TaskRoll( float _delay, uint32_t _roll, uint32_t _max, Scheduler * _scheduler );
	~TaskRoll();

protected:
	bool onRun() override;
	void onSkip() override;

protected:
	void onScheduleComplete( uint32_t _id ) override;
	void onScheduleStop( uint32_t _id ) override;

protected:
	float m_delay;
	uint32_t m_roll;
	uint32_t m_max;

	Scheduler * m_scheduler;
	uint32_t m_id;
};
