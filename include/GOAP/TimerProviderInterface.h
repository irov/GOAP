/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Factorable.h"
#include "GOAP/IntrusivePtr.h"
#include "GOAP/Vector.h"
#include "GOAP/Allocator.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class TimerProviderInterface
        : public Factorable
    {
    protected:
        TimerProviderInterface( Allocator * _allocator )
            : Factorable( _allocator )
        {
        }

    public:
        virtual void onTime( float _time ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TimerProviderInterface> TimerProviderInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class TimerProviderT
        : public TimerProviderInterface
    {
    public:
        TimerProviderT( Allocator * _allocator, F _f )
            : TimerProviderInterface( _allocator )
            , m_f( _f )
        {
        }

    public:
        void onTime( float _time ) override
        {
            m_f( _time );
        }

    protected:
        F m_f;
    };
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        template<class F>
        TimerProviderInterfacePtr makeTimerProvider( Allocator * _allocator, F _f )
        {
            TimerProviderInterface * provider = _allocator->allocateT<TimerProviderT<F>>( _f );

            return TimerProviderInterfacePtr::from( provider );
        }
    }
}