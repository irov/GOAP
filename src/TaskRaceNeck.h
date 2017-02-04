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
		bool _onCheckRun() const override;
		bool _onCheckSkip() const override;
	};
}