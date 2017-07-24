/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	include "GOAP/TaskDeadLock.h"

namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
	TaskDeadLock::TaskDeadLock()
        : Task( TASK_EVENT_RUN )
	{
	}
	//////////////////////////////////////////////////////////////////////////
	TaskDeadLock::~TaskDeadLock()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	bool TaskDeadLock::_onRun()
	{
		return false;
	}
}