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
    typedef IntrusivePtr<class SourceInterface> SourceInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    class TaskFork
        : public Task
    {
        GOAP_DECLARE_VISITABLE( TaskInterface );

    public:
        explicit TaskFork( const SourceInterfacePtr & _source );
        ~TaskFork() override;

    public:
        const SourceInterfacePtr & getSource() const;

    public:
        bool _onRun( NodeInterface * _node ) override;
        void _onFinally() override;

    protected:
        SourceInterfacePtr m_source;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskFork> TaskForkPtr;
    //////////////////////////////////////////////////////////////////////////
}