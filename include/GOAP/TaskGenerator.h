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
        TaskGenerator( const TimerPtr & _timer, const GeneratorProviderPtr & _provider );
        ~TaskGenerator() override;

    public:
        bool _onRun() override;
        bool _onFastSkip() override;
        void _onFinalize() override;

    protected:
        void onTime( float _time );

    protected:        
        TimerPtr m_timer;
        GeneratorProviderPtr m_provider;

        TimerProviderPtr m_timerProvider;

        uint32_t m_iterator;

        float m_time;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskGenerator> TaskGeneratorPtr;
    //////////////////////////////////////////////////////////////////////////
}