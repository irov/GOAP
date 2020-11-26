/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/TaskInterface.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class WhileProviderInterface> WhileProviderInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    class TaskWhile
        : public TaskInterface
    {
        GOAP_DECLARE_VISITABLE( TaskInterface );

    public:
        TaskWhile( Allocator * _allocator, const WhileProviderInterfacePtr & _providerWhile );
        ~TaskWhile() override;

    public:
        bool _onRun( NodeInterface * _node ) override;
        bool _onFastSkip() override;
        void _onFinalize() override;

    protected:
        WhileProviderInterfacePtr m_provider;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskWhile> TaskWhilePtr;
    //////////////////////////////////////////////////////////////////////////
}