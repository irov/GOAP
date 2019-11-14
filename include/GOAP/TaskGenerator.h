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
    typedef IntrusivePtr<class Timer> TimerPtr;
    typedef IntrusivePtr<class TimerProvider> TimerProviderPtr;
    typedef IntrusivePtr<class GeneratorProvider> GeneratorProviderPtr;    
    //////////////////////////////////////////////////////////////////////////
    class TaskGenerator
        : public Task
    {
        GOAP_DECLARE_VISITABLE( Task );

    public:
        TaskGenerator( float _time, uint32_t _iterator, const TimerPtr & _timer, const GeneratorProviderPtr & _provider );
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

        TimerPtr m_timer;
        GeneratorProviderPtr m_provider;

        TimerProviderPtr m_timerProvider;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskGenerator> TaskGeneratorPtr;
    //////////////////////////////////////////////////////////////////////////
}