/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Config.h"
#include "GOAP/Vector.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class EventProvider> EventProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    class Event
        : public Factorable
    {
    public:
        Event();
        ~Event();

    public:
        void addObserver( const EventProviderPtr & _eventProvider );
        bool removeObserver( const EventProviderPtr & _eventProvider );

    public:
        void call();

    protected:
        struct ProviderDesc
        {
            EventProviderPtr provider;
            bool dead;
        };

        typedef Vector<ProviderDesc> VectorProviders;
        VectorProviders m_providers;
        VectorProviders m_providersAdd;

        uint32_t m_process;

        class FEventFind;
        class FEventDead;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<Event> EventPtr;
}