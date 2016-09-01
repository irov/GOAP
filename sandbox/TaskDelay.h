#	pragma once

#	include "GOAP/Task.h"
#	include "Scheduler.h"

class TaskDelay
	: public GOAP::Task
	, public SchedulerObserver
{
public:
	TaskDelay( float _delay, Scheduler * _scheduler );
	~TaskDelay();
	
protected:
	void onFinalize() override;

protected:
	bool onRun() override;
	void onSkip() override;

protected:
	void onScheduleComplete( uint32_t _id ) override;
	void onScheduleStop( uint32_t _id ) override;

protected:
	float m_delay;

	Scheduler * m_scheduler;
	uint32_t m_id;
};