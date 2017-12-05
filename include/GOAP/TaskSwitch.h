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

	typedef Vector<SourcePtr> VectorSources;

	class TaskSwitch
		: public Task
	{
	public:
		TaskSwitch( const SwitchProviderPtr & _provider, const VectorSources & _sources );
		~TaskSwitch() override;

	public:
		const VectorSources & getSources() const;

	public:
		bool _onRun() override;
        void _onFinally() override;

	protected:
		SwitchProviderPtr m_provider;

		VectorSources m_sources;
	};

	typedef IntrusivePtr<TaskSwitch> TaskSwitchPtr;
}