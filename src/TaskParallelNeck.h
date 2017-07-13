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
		bool _onCheckRun() const override;
		bool _onCheckSkip() const override;
	};
}