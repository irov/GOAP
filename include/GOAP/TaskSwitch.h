/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	pragma once

#   include "GOAP/Config.h"

#	include "GOAP/Task.h"

namespace GOAP
{
	typedef IntrusivePtr<class Source> SourcePtr;
	typedef IntrusivePtr<class SwitchProvider> SwitchProviderPtr;

	typedef Vector<SourcePtr> TVectorSources;

	class TaskSwitch
		: public Task
	{
	public:
		TaskSwitch( const SwitchProviderPtr & _provider, const TVectorSources & _sources );
		~TaskSwitch();

	public:
		const TVectorSources & getSources() const;

	public:
		bool _onRun() override;

	protected:
		SwitchProviderPtr m_provider;

		TVectorSources m_sources;
	};

	typedef IntrusivePtr<TaskSwitch> TaskSwitchPtr;
}