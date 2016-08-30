#	pragma once

#	include "GOAP/Task.h"

namespace GOAP
{
	typedef IntrusivePtr<class Source> SourcePtr;
	typedef IntrusivePtr<class Chain> ChainPtr;

	class TaskRepeat
		: public Task
	{
	public:
		TaskRepeat( const SourcePtr & _repeat, const SourcePtr & _until );
		~TaskRepeat();

	public:
		void onFinalize() override;

	public:
		bool onRun() override;

	protected:
		void repeatComplete_( bool _skip );
		void untilComplete_( bool _skip );		

	protected:
		SourcePtr m_sourceRepeat;
		SourcePtr m_sourceUntil;

		ChainPtr m_chainRepeat;
		ChainPtr m_chainUntil;

		bool m_repeat;
	};
}