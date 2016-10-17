#	pragma once

#	include "GOAP/Task.h"

#	include <vector>

namespace GOAP
{
	typedef IntrusivePtr<class Source> SourcePtr;
	typedef IntrusivePtr<class SwitchProvider> SwitchProviderPtr;

	typedef std::vector<SourcePtr> TVectorSources;

	class TaskSwitch
		: public Task
	{
	public:
		TaskSwitch( const SwitchProviderPtr & _provider, const TVectorSources & _sources );
		~TaskSwitch();

	public:
		TVectorSources & getSources();

	public:
		bool onRun() override;

	protected:
		SwitchProviderPtr m_provider;

		TVectorSources m_sources;
	};

	typedef IntrusivePtr<TaskSwitch> TaskSwitchPtr;
}