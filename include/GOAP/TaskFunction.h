#	pragma once

#	include "GOAP/Task.h"

namespace GOAP
{
	typedef IntrusivePtr<class FunctionProvider> FunctionProviderPtr;

	class TaskFunction
		: public Task
	{
	public:
		TaskFunction( const FunctionProviderPtr & _provider );
		~TaskFunction();

	public:
		bool _onRun() override;

	protected:
		FunctionProviderPtr m_provider;
	};

	typedef IntrusivePtr<TaskFunction> TaskFunctionPtr;
}