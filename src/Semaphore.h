/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/SemaphoreInterface.h"
#include "GOAP/EventInterface.h"
#include "GOAP/EventProviderInterface.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class Semaphore
        : public SemaphoreInterface
    {
    public:
        Semaphore( Allocator * _allocator, const EventInterfacePtr & _event, int32_t _value );
        ~Semaphore() override;

    public:
        void setValue( int32_t _value ) override;
        int32_t getValue() const override;

    public:
        void subtractValue( int32_t _value ) override;
        void addValue( int32_t _value ) override;

    public:
        void addObserverProvider( const EventProviderInterfacePtr & _event ) override;
        void removeObserverProvider( const EventProviderInterfacePtr & _event ) override;
        void clearObserverProviders() override;

    protected:
        EventInterfacePtr m_event;
        int32_t m_value;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<Semaphore> SemaphorePtr;
    //////////////////////////////////////////////////////////////////////////
}