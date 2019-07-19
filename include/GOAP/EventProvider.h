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
    class EventProvider
        : public Factorable
    {
    public:
        virtual bool onEvent() = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<EventProvider> EventProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class EventProviderT
        : public EventProvider
    {
    public:
        explicit EventProviderT( F _f )
            : m_f( _f )
        {
        }

    public:
        bool onEvent() override
        {
            bool result = m_f();

            return result;
        }

    protected:
        F m_f;
    };
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        template<class F>
        EventProviderPtr makeEventProvider( F _f )
        {
            EventProviderPtr provider = new EventProviderT<F>( _f );

            return provider;
        }
    }
}