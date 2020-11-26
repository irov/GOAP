/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Factorable.h"
#include "GOAP/IntrusivePtr.h"

#include "GOAP/TimerProviderInterface.h"

namespace GOAP
{
    class TimerInterface
        : public Factorable
    {
    protected:
        TimerInterface( Allocator * _allocator )
            : Factorable( _allocator )
        {
        }

    public:
        virtual void addTimerProvider( const TimerProviderInterfacePtr & _provider ) = 0;
        virtual void removeTimerProvider( const TimerProviderInterfacePtr & _provider ) = 0;

    public:
        template<class F>
        TimerProviderInterfacePtr addTimer( Allocator * _allocator, F _f )
        {
            TimerProviderInterfacePtr provider = Helper::makeTimerProvider( _allocator, _f );

            this->addTimerProvider( provider );

            return provider;
        }

    public:
        virtual void update( float _time ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TimerInterface> TimerInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
}