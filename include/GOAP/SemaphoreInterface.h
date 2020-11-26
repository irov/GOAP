/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Factorable.h"
#include "GOAP/IntrusivePtr.h"
#include "GOAP/EventProviderInterface.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class SemaphoreInterface
        : public Factorable
    {
    protected:
        SemaphoreInterface( Allocator * _allocator )
            : Factorable( _allocator )
        {
        }

    public:
        virtual void setValue( int32_t _value ) = 0;
        virtual int32_t getValue() const = 0;

    public:
        virtual void subtractValue( int32_t _value ) = 0;
        virtual void addValue( int32_t _value ) = 0;

    public:
        virtual void addObserverProvider( const EventProviderInterfacePtr & _event ) = 0;
        virtual void removeObserverProvider( const EventProviderInterfacePtr & _event ) = 0;
        virtual void clearObserverProviders() = 0;

    public:
        template<class F>
        EventProviderInterfacePtr addProvider( Allocator * _allocator, F _f )
        {
            EventProviderInterfacePtr provider = Helper::makeEventProvider( _allocator, _f );

            this->addObserverProvider( provider );

            return provider;
        }
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<SemaphoreInterface> SemaphoreInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
}