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

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class TimerProvider
        : public Factorable
    {
    public:
        virtual void onTime( float _time ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TimerProvider> TimerProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class TimerProviderT
        : public TimerProvider
    {
    public:
        explicit TimerProviderT( F _f )
            : m_f( _f )
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
        TimerProviderPtr makeTimerProvider( F _f )
        {
            return TimerProviderPtr::from( new TimerProviderT<F>( _f ) );
        }
    }
}