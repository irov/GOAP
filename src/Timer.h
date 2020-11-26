/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/TimerInterface.h"
#include "GOAP/Vector.h"
#include "GOAP/Allocator.h"

namespace GOAP
{
    class Timer
        : public TimerInterface
    {
    public:
        Timer( Allocator * _allocator );
        ~Timer() override;

    public:
        void addTimerProvider( const TimerProviderInterfacePtr & _provider ) override;
        void removeTimerProvider( const TimerProviderInterfacePtr & _provider ) override;

    public:
        void update( float _time ) override;

    protected:
        struct TimerDesc
        {
            TimerProviderInterfacePtr provider;
            bool dead;
        };

        typedef Vector<TimerDesc> VectorTimerProvider;
        VectorTimerProvider m_providerAdd;
        VectorTimerProvider m_provider;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<Timer> TimerPtr;
    //////////////////////////////////////////////////////////////////////////
}