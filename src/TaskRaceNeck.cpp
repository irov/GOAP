#	include "TaskRaceNeck.h"

namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
	TaskRaceNeck::TaskRaceNeck()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	TaskRaceNeck::~TaskRaceNeck()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	bool TaskRaceNeck::onCheckRun() const
	{
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool TaskRaceNeck::onCheckSkip() const
	{
		return false;
	}
}