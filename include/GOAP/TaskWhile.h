/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
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
    class TaskWhile
        : public Task
    {
        DECLARE_VISITABLE( Task );

    public:
        explicit TaskWhile( const ScopeProviderPtr & _providerWhile );
        ~TaskWhile() override;

    public:
        bool _onRun() override;
        void _onFinalize() override;

    protected:
        ScopeProviderPtr m_providerWhile;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskWhile> TaskWhilePtr;
    //////////////////////////////////////////////////////////////////////////
}