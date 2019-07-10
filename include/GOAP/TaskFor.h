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
	typedef IntrusivePtr<class ForProvider> ForProviderPtr;
    //////////////////////////////////////////////////////////////////////////
	class TaskFor
		: public Task
	{
        DECLARE_VISITABLE( Task );

	public:
        TaskFor( const ForProviderPtr & _providerFor, uint32_t _iterator, uint32_t _count );
		~TaskFor() override;

	public:
        bool _onCheck() override;
		bool _onRun() override;
        void _onFinally() override;

	protected:
        ForProviderPtr m_providerFor;
        uint32_t m_iterator;
        uint32_t m_count;
	};
	//////////////////////////////////////////////////////////////////////////
	typedef IntrusivePtr<TaskFor> TaskForPtr;
	//////////////////////////////////////////////////////////////////////////
}