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
    typedef IntrusivePtr<class SourceInterface> SourceInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    class TaskRepeat
        : public TaskInterface
    {
        GOAP_DECLARE_VISITABLE( TaskInterface );

    public:
        TaskRepeat( Allocator * _allocator, const WhileProviderInterfacePtr & _provider, const SourceInterfacePtr & _until );
        ~TaskRepeat() override;

    public:
        bool _onRun( NodeInterface * _node ) override;
        void _onFinalize() override;

    protected:
        WhileProviderInterfacePtr m_provider;
        SourceInterfacePtr m_sourceUntil;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskRepeat> TaskRepeatPtr;
    //////////////////////////////////////////////////////////////////////////
}