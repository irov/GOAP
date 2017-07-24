/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

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
		bool _onRun() override;
	};
}