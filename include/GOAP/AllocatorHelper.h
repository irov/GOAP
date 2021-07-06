/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Allocator.h"
#include "GOAP/IntrusivePtr.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class NodeInterface> NodeInterfacePtr;
    typedef IntrusivePtr<class TaskInterface> TaskInterfacePtr;
    typedef IntrusivePtr<class ChainInterface> ChainInterfacePtr;
    typedef IntrusivePtr<class SourceInterface> SourceInterfacePtr;
    typedef IntrusivePtr<class SourceProviderInterface> SourceProviderInterfacePtr;
    typedef IntrusivePtr<class EventInterface> EventInterfacePtr;
    typedef IntrusivePtr<class SemaphoreInterface> SemaphoreInterfacePtr;
    typedef IntrusivePtr<class SourceInterface> SourceInterfacePtr;
    typedef IntrusivePtr<class TimerInterface> TimerInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        NodeInterfacePtr makeNode( Allocator * _allocator, const TaskInterfacePtr & _task );
        ChainInterfacePtr makeChain( Allocator * _allocator, const SourceInterfacePtr & _source, const char * _file, uint32_t _line );
        EventInterfacePtr makeEvent( Allocator * _allocator );
        SemaphoreInterfacePtr makeSemaphore( Allocator * _allocator, const EventInterfacePtr & _event, int32_t _value );
        SourceProviderInterfacePtr makeSourceProvider( Allocator * _allocator );
        SourceInterfacePtr makeSource( Allocator * _allocator );
        TimerInterfacePtr makeTimer( Allocator * _allocator );
    }
    //////////////////////////////////////////////////////////////////////////
}