/*
* Copyright (C) 2017, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	pragma once

#	include "GOAP/Task.h"

namespace GOAP
{
	class TaskParallelNeck
		: public Task
	{
	public:
		TaskParallelNeck();
		~TaskParallelNeck() override;

	protected:
		bool _onCheckRun() const override;
		bool _onCheckSkip() const override;
	};
}