/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Factorable.h"
#include "GOAP/IntrusivePtr.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class CallbackObserver
        : public Factorable
    {
    public:
        virtual void onCallback( bool _skip ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<CallbackObserver> CallbackObserverPtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class CallbackObserverT
        : public CallbackObserver
    {
    public:
        explicit CallbackObserverT( F _f )
            : m_f( _f )
        {
        }

    public:
        void onCallback( bool _skip ) override
        {
            m_f( _skip );
        }

    protected:
        F m_f;
    };
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        template<class F>
        CallbackObserverPtr makeCallbackObserver( Allocator * _allocator, F _f )
        {
            CallbackObserver * observer = _allocator->allocateT<CallbackObserverT<F>>( _f );

            return CallbackObserverPtr::from( observer );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}