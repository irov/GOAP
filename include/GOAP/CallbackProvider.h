/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/CallbackObserver.h"

namespace GOAP
{ 
    //////////////////////////////////////////////////////////////////////////
    class CallbackProvider
        : public Factorable
    {
    public:
        virtual void onCallbackProvider( const CallbackObserverPtr & _callback, bool _skip ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<CallbackProvider> CallbackProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class CallbackProviderT
        : public CallbackProvider
    {
    public:
        explicit CallbackProviderT( F _f )
            : m_f( _f )
        {
        }

    public:
        void onCallbackProvider( const CallbackObserverPtr & _callback, bool _skip ) override
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
        CallbackProviderPtr makeCallbackProvider( F _f )
        {
            return CallbackProviderPtr::from( new CallbackProviderT<F>( _f ) );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}