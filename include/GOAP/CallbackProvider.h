/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Config.h"

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
    class CallbackProvider
        : public Factorable
    {
    public:
        virtual void onCallback( const CallbackObserverPtr & _callback, bool _skip ) = 0;
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
        void onCallback( const CallbackObserverPtr & _callback, bool _skip ) override
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
            CallbackProviderPtr provider = new CallbackProviderT<F>( _f );

            return provider;
        }
    }
}