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
		TVectorSources & getSources();

	public:
		bool _onRun() override;

	protected:
		SwitchProviderPtr m_provider;

		TVectorSources m_sources;
	};

	typedef IntrusivePtr<TaskSwitch> TaskSwitchPtr;
}