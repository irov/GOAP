/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "Kernel.h"

#include "Node.h"
#include "Chain.h"
#include "Event.h"
#include "Semaphore.h"
#include "SourceProvider.h"
#include "Source.h"
#include "Timer.h"

namespace GOAP
{ 
    //////////////////////////////////////////////////////////////////////////
    Kernel::Kernel()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Kernel::~Kernel()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    NodeInterfacePtr Kernel::makeNode( const TaskInterfacePtr & _task )
    {
        Allocator * allocator = this->getAllocator();

        Node * node = allocator->allocateT<Node>( _task );

        return NodeInterfacePtr::from( node );
    }
    //////////////////////////////////////////////////////////////////////////
    ChainInterfacePtr Kernel::makeChain( const SourceInterfacePtr & _source, const char * _file, uint32_t _line )
    {
        Allocator * allocator = this->getAllocator();

        Chain * chain = allocator->allocateT<Chain>( _source, _file, _line );

        return ChainInterfacePtr::from( chain );
    }
    //////////////////////////////////////////////////////////////////////////
    EventInterfacePtr Kernel::makeEvent()
    {
        Allocator * allocator = this->getAllocator();

        Event * event = allocator->allocateT<Event>();

        return EventInterfacePtr::from( event );
    }
    //////////////////////////////////////////////////////////////////////////
    SemaphoreInterfacePtr Kernel::makeSemaphore( const EventInterfacePtr & _event, int32_t _value )
    {
        Allocator * allocator = this->getAllocator();

        Semaphore * semaphore = allocator->allocateT<Semaphore>( _event, _value );

        return SemaphoreInterfacePtr::from( semaphore );
    }
    //////////////////////////////////////////////////////////////////////////
    SourceProviderInterfacePtr Kernel::makeSourceProvider()
    {
        Allocator * allocator = this->getAllocator();

        SourceProvider * provider = allocator->allocateT<SourceProvider>();

        return SourceProviderInterfacePtr::from( provider );
    }
    //////////////////////////////////////////////////////////////////////////
    SourceInterfacePtr Kernel::makeSource()
    {
        SourceProviderInterfacePtr provider = this->makeSourceProvider();

        Allocator * allocator = this->getAllocator();

        Source * source = allocator->allocateT<Source>( KernelInterfacePtr::from( this ), provider );

        return SourceInterfacePtr::from( source );
    }
    //////////////////////////////////////////////////////////////////////////
    TimerInterfacePtr Kernel::makeTimer()
    {
        Allocator * allocator = this->getAllocator();

        Timer * timer = allocator->allocateT<Timer>();

        return TimerInterfacePtr::from( timer );
    }
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        KernelInterfacePtr makeKernel( Allocator * _allocator )
        {
            Kernel * kernel = _allocator->allocateT<Kernel>();

            return KernelInterfacePtr::from( kernel );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}