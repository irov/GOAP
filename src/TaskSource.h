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
    typedef IntrusivePtr<class SourceInterface> SourceInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    class TaskSource
        : public TaskInterface
    {
        GOAP_DECLARE_VISITABLE( TaskInterface );

    public:
        TaskSource( Allocator * _allocator, const SourceInterfacePtr & _source );
        ~TaskSource() override;

    public:
        bool _onRun( NodeInterface * _node ) override;
        void _onFinally() override;

    protected:
        SourceInterfacePtr m_source;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskSource> TaskSourcePtr;
    //////////////////////////////////////////////////////////////////////////
}