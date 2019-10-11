/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/TimerProvider.h"

#include "GOAP/Vector.h"

namespace GOAP
{    
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class TimerProvider> TimerProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    class Timer
        : public Factorable
    {
    public:
        Timer();
        ~Timer() override;

    public:
        void addTimerProvider( const TimerProviderPtr & _provider );
        void removeTimerProvider( const TimerProviderPtr & _provider );

    public:
        template<class F>
        TimerProviderPtr addTimer( F _f )
        {
            TimerProviderPtr provider = Helper::makeTimerProvider( _f );

            this->addTimerProvider( provider );

            return provider;
        }

    public:
        void update( float _time );

    protected:
        struct TimerDesc
        {            
            TimerProviderPtr provider;
            bool dead;
        };

        typedef Vector<TimerDesc> VectorTimerProvider;
        VectorTimerProvider m_providerAdd;
        VectorTimerProvider m_provider;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<Timer> TimerPtr;
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        inline TimerPtr makeTimer()
        {
            return TimerPtr::from( new Timer() );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}