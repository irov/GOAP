/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	pragma once

#	include "GOAP/Task.h"

namespace GOAP
{
	typedef IntrusivePtr<class ScopeProvider> ScopeProviderPtr;
	typedef IntrusivePtr<class Source> SourcePtr;
	typedef IntrusivePtr<class Chain> ChainPtr;

	class TaskWhile
		: public Task
	{
	public:
        explicit TaskWhile( const ScopeProviderPtr & _providerScope );
		~TaskWhile() override;

	public:
		void _onFinalize() override;

	public:
		bool _onRun() override;

	protected:
		void whileComplete_( bool _skip );

	protected:
		ScopeProviderPtr m_providerScope;

		ChainPtr m_chainWhile;
	};
}