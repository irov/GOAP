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
    typedef IntrusivePtr<class WhileProvider> WhileProviderPtr;
    typedef IntrusivePtr<class SourceInterface> SourceInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    class TaskRepeat
        : public Task
    {
        GOAP_DECLARE_VISITABLE( Task );

    public:
        TaskRepeat( const WhileProviderPtr & _provider, const SourceInterfacePtr & _until );
        ~TaskRepeat() override;

    public:
        bool _onRun( NodeInterface * _task ) override;
        void _onFinalize() override;

    protected:
        WhileProviderPtr m_provider;
        SourceInterfacePtr m_sourceUntil;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskRepeat> TaskRepeatPtr;
    //////////////////////////////////////////////////////////////////////////
}