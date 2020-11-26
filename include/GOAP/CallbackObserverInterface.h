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
    class CallbackObserverInterface
        : public Factorable
    {
    protected:
        CallbackObserverInterface( Allocator * _allocator )
            : Factorable( _allocator )
        {
        }

    public:
        virtual void onCallback( bool _skip ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<CallbackObserverInterface> CallbackObserverInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class CallbackObserverT
        : public CallbackObserverInterface
    {
    public:
        CallbackObserverT( Allocator * _allocator, F _f )
            : CallbackObserverInterface( _allocator )
            , m_f( _f )
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
        CallbackObserverInterfacePtr makeCallbackObserver( Allocator * _allocator, F _f )
        {
            CallbackObserverInterface * observer = _allocator->allocateT<CallbackObserverT<F>>( _f );

            return CallbackObserverInterfacePtr::from( observer );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}