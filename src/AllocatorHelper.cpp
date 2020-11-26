/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/AllocatorHelper.h"

#include "Node.h"
#include "Chain.h"
#include "Event.h"
#include "Semaphore.h"
#include "SourceProvider.h"
#include "Source.h"
#include "Timer.h"

namespace GOAP
{
    namespace Helper
    {
        //////////////////////////////////////////////////////////////////////////
        NodeInterfacePtr makeNode( Allocator * _allocator, const TaskInterfacePtr & _task )
        {
            Node * node = _allocator->allocateT<Node>( _task );

            return NodeInterfacePtr::from( node );
        }
        //////////////////////////////////////////////////////////////////////////
        ChainInterfacePtr makeChain( Allocator * _allocator, const SourceInterfacePtr & _source, const char * _file, uint32_t _line )
        {
            Chain * chain = _allocator->allocateT<Chain>( _source, _file, _line );

            return ChainInterfacePtr::from( chain );
        }
        //////////////////////////////////////////////////////////////////////////
        EventInterfacePtr makeEvent( Allocator * _allocator )
        {
            Event * event = _allocator->allocateT<Event>();

            return EventInterfacePtr::from( event );
        }
        //////////////////////////////////////////////////////////////////////////
        SemaphoreInterfacePtr makeSemaphore( Allocator * _allocator, const EventInterfacePtr & _event, int32_t _value )
        {
            Semaphore * semaphore = _allocator->allocateT<Semaphore>( _event, _value );

            return SemaphoreInterfacePtr::from( semaphore );
        }
        //////////////////////////////////////////////////////////////////////////
        SourceProviderInterfacePtr makeSourceProvider( Allocator * _allocator )
        {
            SourceProvider * provider = _allocator->allocateT<SourceProvider>();

            return SourceProviderInterfacePtr::from( provider );
        }
        //////////////////////////////////////////////////////////////////////////
        SourceInterfacePtr makeSource( Allocator * _allocator )
        {
            SourceProviderInterfacePtr provider = Helper::makeSourceProvider( _allocator );

            Source * source = _allocator->allocateT<Source>( provider );

            return SourceInterfacePtr::from( source );
        }
        //////////////////////////////////////////////////////////////////////////
        TimerInterfacePtr makeTimer( Allocator * _allocator )
        {
            Timer * timer = _allocator->allocateT<Timer>();

            return TimerInterfacePtr::from( timer );
        }
    }
}