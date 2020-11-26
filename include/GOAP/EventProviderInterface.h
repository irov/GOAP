/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Factorable.h"
#include "GOAP/IntrusivePtr.h"
#include "GOAP/Allocator.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class EventProviderInterface
        : public Factorable
    {
    public:
        EventProviderInterface( Allocator * _allocator )
            : Factorable( _allocator )
        {
        }

    public:
        virtual bool onEvent() = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<EventProviderInterface> EventProviderInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    template<class F>
    class EventProviderT
        : public EventProviderInterface
    {
    public:
        EventProviderT( Allocator * _allocator, F _f )
            : EventProviderInterface( _allocator )
            , m_f( _f )
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
        EventProviderInterfacePtr makeEventProvider( Allocator * _allocator, F _f )
        {
            EventProviderInterface * provider = _allocator->allocateT<EventProviderT<F>>( _f );

            return EventProviderInterfacePtr::from( provider );
        }
    }
}