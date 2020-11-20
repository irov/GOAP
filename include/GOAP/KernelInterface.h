/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Config.h"
#include "GOAP/IntrusivePtr.h"
#include "GOAP/Factorable.h"
#include "GOAP/Allocator.h"

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
    class KernelInterface
        : public Factorable
    {
    public:
        virtual NodeInterfacePtr makeNode( const TaskInterfacePtr & _task ) = 0;
        virtual ChainInterfacePtr makeChain( const SourceInterfacePtr & _source, const char * _file, uint32_t _line ) = 0;
        virtual EventInterfacePtr makeEvent() = 0;
        virtual SemaphoreInterfacePtr makeSemaphore( const EventInterfacePtr & _event, int32_t _value ) = 0;
        virtual SourceInterfacePtr makeSource() = 0;
        virtual SourceProviderInterfacePtr makeSourceProvider() = 0;
        virtual TimerInterfacePtr makeTimer() = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<KernelInterface> KernelInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        KernelInterfacePtr makeKernel( Allocator * _allocator );
    }
    //////////////////////////////////////////////////////////////////////////
}