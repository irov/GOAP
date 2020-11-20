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
    class EventInterface
        : public Factorable
    {
    public:
        virtual void addProvider( const EventProviderInterfacePtr & _eventProvider ) = 0;
        virtual bool removeProvider( const EventProviderInterfacePtr & _eventProvider ) = 0;
        virtual void clearProviders() = 0;

    public:
        virtual void call() = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<EventInterface> EventInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
}