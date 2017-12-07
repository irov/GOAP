/*
* Copyright (C) 2017, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	pragma once

#	include "GOAP/Task.h"
#	include "GOAP/Semaphore.h"

namespace GOAP
{
    static const uint32_t TASK_SEMAPHORE_WAIT = 0x00000001;
    static const uint32_t TASK_SEMAPHORE_FROM = 0x00000002;
    static const uint32_t TASK_SEMAPHORE_LESS = 0x00000004;
    static const uint32_t TASK_SEMAPHORE_TO = 0x00000010;
    static const uint32_t TASK_SEMAPHORE_ADD = 0x00000020;
    static const uint32_t TASK_SEMAPHORE_SUBTRACT = 0x00000040;

    class TaskSemaphore
        : public Task
    {
    public:
        TaskSemaphore( const SemaphorePtr & _semaphore, uint32_t _flags, int32_t _test, int32_t _apply );
        ~TaskSemaphore() override;

    public:
        bool _onCheck() override;
        bool _onRun() override;
        void _onComplete() override;
        void _onFinally() override;

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

    typedef IntrusivePtr<TaskSemaphore> TaskSemaphorePtr;
}