/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/TaskInterface.h"

#include "GOAP/TimerInterface.h"
#include "GOAP/GeneratorProviderInterface.h"

#include "GOAP/Allocator.h"

namespace GOAP
{
    class TaskGenerator
        : public TaskInterface
    {
        GOAP_DECLARE_VISITABLE( TaskInterface );

    public:
        TaskGenerator( float _time, uint32_t _iterator, const TimerInterfacePtr & _timer, const GeneratorProviderInterfacePtr & _provider );
        ~TaskGenerator() override;

    public:
        bool _onRun( NodeInterface * _node ) override;
        bool _onFastSkip() override;
        void _onCancel() override;
        void _onFinalize() override;

    protected:
        void onTime( NodeInterface * _node, float _time );

    protected:
        float m_time;
        uint32_t m_iterator;

        TimerInterfacePtr m_timer;
        GeneratorProviderInterfacePtr m_provider;

        TimerProviderInterfacePtr m_timerProvider;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskGenerator> TaskGeneratorPtr;
    //////////////////////////////////////////////////////////////////////////
}