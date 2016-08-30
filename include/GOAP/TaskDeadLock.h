#	pragma once

#	include "GOAP/Task.h"

namespace GOAP
{
	class TaskDeadLock
		: public Task
	{
	public:
		TaskDeadLock();
		~TaskDeadLock();

	protected:
		bool onRun() override;
	};
}