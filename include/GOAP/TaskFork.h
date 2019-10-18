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
    typedef IntrusivePtr<class Source> SourcePtr;
    //////////////////////////////////////////////////////////////////////////
    class TaskFork
        : public Task
    {
        GOAP_DECLARE_VISITABLE( TaskInterface );

    public:
        explicit TaskFork( const SourcePtr & _source );
        ~TaskFork() override;

    public:
        bool _onRun( NodeInterface * _task ) override;
        void _onFinally() override;

    protected:
        SourcePtr m_source;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskFork> TaskForkPtr;
    //////////////////////////////////////////////////////////////////////////
}