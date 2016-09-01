#	pragma once

#	include "GOAP/Task.h"

namespace GOAP
{
	class TaskRaceNeck
		: public Task
	{
	public:
		TaskRaceNeck();
		~TaskRaceNeck();

	protected:
		bool onCheckRun() const override;
		bool onCheckSkip() const override;
	};
}