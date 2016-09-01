#	pragma once

#	include "GOAP/Task.h"

#	include "GOAP/CallbackProvider.h"

namespace GOAP
{
	class TaskCallback
		: public Task
		, public CallbackObserver
	{
	public:
		TaskCallback( const CallbackProviderPtr & _provider );
		~TaskCallback();

	public:
		bool onRun() override;

	protected:
		void onCallback( bool _skip ) override;
		
	protected:
		CallbackProviderPtr m_provider;
	};
}