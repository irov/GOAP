/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Kernel.h"

#include "GOAP/Node.h"
#include "GOAP/Task.h"
#include "GOAP/Chain.h"
#include "GOAP/Event.h"
#include "GOAP/Semaphore.h"
#include "GOAP/SourceProvider.h"
#include "GOAP/Source.h"
#include "GOAP/Timer.h"

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
        Node * node = m_allocator->allocateT<Node>( _task );

        return NodeInterfacePtr::from( node );
    }
    //////////////////////////////////////////////////////////////////////////
    ChainInterfacePtr Kernel::makeChain( const SourceInterfacePtr & _source, const char * _file, uint32_t _line )
    {
        Chain * chain = m_allocator->allocateT<Chain>( m_allocator, _source, _file, _line );

        return ChainInterfacePtr::from( chain );
    }
    //////////////////////////////////////////////////////////////////////////
    EventInterfacePtr Kernel::makeEvent()
    {
        Event * event = m_allocator->allocateT<Event>();

        return EventInterfacePtr::from( event );
    }
    //////////////////////////////////////////////////////////////////////////
    SemaphoreInterfacePtr Kernel::makeSemaphore( const EventInterfacePtr & _event, int32_t _value )
    {
        Semaphore * semaphore = m_allocator->allocateT<Semaphore>( m_allocator, _event, _value );

        return SemaphoreInterfacePtr::from( semaphore );
    }
    //////////////////////////////////////////////////////////////////////////
    SourceProviderInterfacePtr Kernel::makeSourceProvider()
    {
        SourceProvider * provider = m_allocator->allocateT<SourceProvider>();

        return SourceProviderInterfacePtr::from( provider );
    }
    //////////////////////////////////////////////////////////////////////////
    SourceInterfacePtr Kernel::makeSource()
    {
        SourceProviderInterfacePtr provider = this->makeSourceProvider();

        Source * source = m_allocator->allocateT<Source>( KernelInterfacePtr::from( this ), provider );

        return SourceInterfacePtr::from( source );
    }
    //////////////////////////////////////////////////////////////////////////
    TimerInterfacePtr Kernel::makeTimer()
    {
        Timer * timer = m_allocator->allocateT<Timer>();

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