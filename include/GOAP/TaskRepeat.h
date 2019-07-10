/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Task.h"

namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
	typedef IntrusivePtr<class ScopeProvider> ScopeProviderPtr;
	typedef IntrusivePtr<class Source> SourcePtr;
	typedef IntrusivePtr<class Chain> ChainPtr;
	//////////////////////////////////////////////////////////////////////////
	class TaskRepeat
		: public Task
	{
        DECLARE_VISITABLE( Task );

	public:
		TaskRepeat( const ScopeProviderPtr & _provider, const SourcePtr & _until );
		~TaskRepeat() override;

	public:
		void _onFinalize() override;

	public:
		bool _onRun() override;

	protected:
		void repeatComplete_( bool _skip );
		void untilComplete_( bool _skip );

	protected:
		ScopeProviderPtr m_providerRepeat;
		SourcePtr m_sourceUntil;

		ChainPtr m_chainRepeat;
		ChainPtr m_chainUntil;

		bool m_repeat;

        class ChainProviderUntilEnd;
        class ChainProviderRepeatEnd;
	};
	//////////////////////////////////////////////////////////////////////////
	typedef IntrusivePtr<TaskRepeat> TaskRepeatPtr;
	//////////////////////////////////////////////////////////////////////////
}