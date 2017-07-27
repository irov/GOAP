/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	pragma once

#	include "GOAP/Task.h"
#   include "GOAP/Factory.h"

namespace GOAP
{
	typedef IntrusivePtr<class ScopeProvider> ScopeProviderPtr;

	class TaskScope
		: public Task
	{
	public:
		TaskScope( const FactoryPtr & _factory, const ScopeProviderPtr & _provider );
		~TaskScope();

	public:
		bool _onRun() override;

	protected:
        FactoryPtr m_factory;
		ScopeProviderPtr m_provider;
	};
}