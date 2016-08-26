#	pragma once

#	include "GOAP/Task.h"

namespace GOAP
{
	class TaskParallelNeck
		: public Task
	{
	public:
		TaskParallelNeck();
		~TaskParallelNeck();

	protected:
		bool onCheckRun() const override;
		bool onCheckSkip() const override;
	};
}