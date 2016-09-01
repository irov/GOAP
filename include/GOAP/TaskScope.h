#	pragma once

#	include "GOAP/Task.h"

namespace GOAP
{
	typedef IntrusivePtr<class ScopeProvider> ScopeProviderPtr;

	class TaskScope
		: public Task
	{
	public:
		TaskScope( const ScopeProviderPtr & _provider );
		~TaskScope();

	public:
		bool onRun() override;

	protected:
		ScopeProviderPtr m_provider;
	};
}