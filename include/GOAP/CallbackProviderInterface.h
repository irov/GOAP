/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/CallbackObserverInterface.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class CallbackProviderInterface
        : public Factorable
    {
    protected:
        CallbackProviderInterface( Allocator * _allocator )
            : Factorable( _allocator )
        {
        }

    public:
        virtual void onCallbackProvider( const CallbackObserverInterfacePtr & _callback, bool _skip ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<CallbackProviderInterface> CallbackProviderInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class CallbackProviderT
        : public CallbackProviderInterface
    {
    public:
        CallbackProviderT( Allocator * _allocator, F _f )
            : CallbackProviderInterface( _allocator )
            , m_f( _f )
        {
        }

    public:
        void onCallbackProvider( const CallbackObserverInterfacePtr & _callback, bool _skip ) override
        {
            m_f( _callback, _skip );
        }

    protected:
        F m_f;
    };
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        template<class F>
        CallbackProviderInterfacePtr makeCallbackProvider( Allocator * _allocator, F _f )
        {
            CallbackProviderInterface * provider = _allocator->allocateT<CallbackProviderT<F>>( _f );

            return CallbackProviderInterfacePtr::from( provider );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}