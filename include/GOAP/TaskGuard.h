#	pragma once

#	include "GOAP/Task.h"

namespace GOAP
{
	typedef IntrusivePtr<class GuardProvider> GuardProviderPtr;

	class TaskGuard
		: public Task
	{
	public:
		TaskGuard(const GuardProviderPtr & _begin, const GuardProviderPtr & _end);
		~TaskGuard();

	public:
		bool _onRun() override;
		void _onCancel() override;

	protected:
		GuardProviderPtr m_begin;
		GuardProviderPtr m_end;
	};

	typedef IntrusivePtr<TaskGuard> TaskGuardPtr;
}