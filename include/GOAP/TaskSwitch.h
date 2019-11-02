/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Task.h"
#include "GOAP/Vector.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class SourceInterface> SourceInterfacePtr;
    typedef IntrusivePtr<class SwitchProvider> SwitchProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    typedef Vector<SourceInterfacePtr> VectorSources;
    //////////////////////////////////////////////////////////////////////////
    class TaskSwitch
        : public Task
    {
        GOAP_DECLARE_VISITABLE( Task );        

    public:
        TaskSwitch( const SwitchProviderPtr & _provider, VectorSources && _sources );
        ~TaskSwitch() override;

    public:
        const VectorSources & getSources() const;

    public:
        bool _onRun( NodeInterface * _node ) override;
        void _onFinally() override;

    protected:
        SwitchProviderPtr m_provider;

        VectorSources m_sources;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskSwitch> TaskSwitchPtr;
    //////////////////////////////////////////////////////////////////////////
}