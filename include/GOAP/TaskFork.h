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
        DECLARE_VISITABLE( Task );

    public:
        explicit TaskFork( const SourcePtr & _fork );
        ~TaskFork() override;

    public:
        bool _onRun() override;
        void _onFinally() override;

    protected:
        SourcePtr m_fork;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskFork> TaskForkPtr;
    //////////////////////////////////////////////////////////////////////////
}