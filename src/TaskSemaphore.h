/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/TaskInterface.h"
#include "GOAP/SemaphoreInterface.h"
#include "GOAP/SemaphoreFlags.h"

namespace GOAP
{
    class TaskSemaphore
        : public TaskInterface
    {
        GOAP_DECLARE_VISITABLE( TaskInterface );

    public:
        TaskSemaphore( Allocator * _allocator, const SemaphoreInterfacePtr & _semaphore, uint32_t _flags, int32_t _test, int32_t _apply );
        ~TaskSemaphore() override;

    public:
        bool _onCheck() override;
        bool _onRun( NodeInterface * _node ) override;
        void _onComplete() override;
        void _onFinally() override;
        bool _onSkipable() const override;

    protected:
        bool test( NodeInterface * _node );
        void process();

    protected:
        SemaphoreInterfacePtr m_semaphore;
        uint32_t m_flags;
        int32_t m_test;
        int32_t m_apply;

        EventProviderInterfacePtr m_provider;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskSemaphore> TaskSemaphorePtr;
    //////////////////////////////////////////////////////////////////////////
}