/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Factorable.h"
#include "GOAP/IntrusivePtr.h"
#include "GOAP/Event.h"
#include "GOAP/EventProvider.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class Semaphore
        : public Factorable
    {
    public:
        Semaphore( const EventPtr & _event, int32_t _value );
        ~Semaphore() override;

    public:
        void setValue( int32_t _value );
        int32_t getValue() const;

    public:
        void subtractValue( int32_t _value );
        void addValue( int32_t _value );

    public:
        void addObserverProvider( const EventProviderPtr & _event );
        void removeObserverProvider( const EventProviderPtr & _event );

    public:
        template<class F>
        EventProviderPtr addObserver( F _f )
        {
            EventProviderPtr provider = Helper::makeEventProvider( _f );

            this->addObserverProvider( provider );

            return provider;
        }

    protected:
        EventPtr m_event;
        int32_t m_value;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<Semaphore> SemaphorePtr;
    //////////////////////////////////////////////////////////////////////////
}