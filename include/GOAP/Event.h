/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/EventInterface.h"

#include "GOAP/Vector.h"
#include "GOAP/Allocator.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class EventProviderInterface> EventProviderInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    class Event
        : public EventInterface
    {
    public:
        Event();
        ~Event() override;

    public:
        void addProvider( const EventProviderInterfacePtr & _eventProvider ) override;
        bool removeProvider( const EventProviderInterfacePtr & _eventProvider ) override;

    public:
        void call() override;

    protected:
        struct ProviderDesc
        {
            EventProviderInterfacePtr provider;
            bool dead;
        };

        typedef Vector<ProviderDesc> VectorProviders;
        VectorProviders m_providers;
        VectorProviders m_providersAdd;

        uint32_t m_process;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<Event> EventPtr;
    //////////////////////////////////////////////////////////////////////////
}