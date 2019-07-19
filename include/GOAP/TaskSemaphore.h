/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Task.h"
#include "GOAP/Semaphore.h"
#include "GOAP/SemaphoreFlags.h"

namespace GOAP
{
    class TaskSemaphore
        : public Task
    {
        DECLARE_VISITABLE( Task );

    public:
        TaskSemaphore( const SemaphorePtr & _semaphore, uint32_t _flags, int32_t _test, int32_t _apply );
        ~TaskSemaphore() override;

    public:
        bool _onCheck() override;
        bool _onRun() override;
        void _onComplete() override;
        void _onFinally() override;
        bool _onSkipable() const override;

    protected:
        bool test();
        void process();

    protected:
        SemaphorePtr m_semaphore;
        uint32_t m_flags;
        int32_t m_test;
        int32_t m_apply;

        EventProviderPtr m_observer;

        class EventProviderTaskSemaphore;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskSemaphore> TaskSemaphorePtr;
    //////////////////////////////////////////////////////////////////////////
}