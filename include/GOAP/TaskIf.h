/*
* Copyright (C) 2017, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	pragma once

#	include "GOAP/Task.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
	typedef IntrusivePtr<class Source> SourcePtr;
	typedef IntrusivePtr<class IfProvider> IfProviderPtr;
    //////////////////////////////////////////////////////////////////////////
	class TaskIf
		: public Task
	{
	public:
		TaskIf( const IfProviderPtr & _provider, const SourcePtr & _sourceTrue, const SourcePtr & _sourceFalse );
		~TaskIf() override;

	public:
		bool _onRun() override;
        void _onFinally() override;

	protected:
		IfProviderPtr m_provider;

		SourcePtr m_sourceTrue;
		SourcePtr m_sourceFalse;
	};
}