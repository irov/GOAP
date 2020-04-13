/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Factorable.h"
#include "GOAP/IntrusivePtr.h"
#include "GOAP/ArraySources.h"
#include "GOAP/VectorSources.h"
#include "GOAP/ViewSources.h"
#include "GOAP/Zip.h"

#include "GOAP/SourceInterface.h"

#include "GOAP/TimerInterface.h"
#include "GOAP/SemaphoreInterface.h"
#include "GOAP/EventInterface.h"
#include "GOAP/FunctionProviderInterface.h"
#include "GOAP/FunctionContextProviderInterface.h"
#include "GOAP/CallbackProviderInterface.h"
#include "GOAP/ScopeProviderInterface.h"
#include "GOAP/WhileProviderInterface.h"
#include "GOAP/ForProviderInterface.h"
#include "GOAP/GeneratorProviderInterface.h"
#include "GOAP/TriggerProviderInterface.h"
#include "GOAP/IfProviderInterface.h"
#include "GOAP/IfProviderInterface.h"
#include "GOAP/GuardProviderInterface.h"
#include "GOAP/SwitchProviderInterface.h"
#include "GOAP/TranscriptorParallelArray.h"
#include "GOAP/TranscriptorRaceArray.h"

namespace GOAP
{
    namespace Cook
    {
        //////////////////////////////////////////////////////////////////////////
        void addBlock( const SourceInterfacePtr & _source );
        void addNoSkip( const SourceInterfacePtr & _source );
        void addSource( const SourceInterfacePtr & _source, const SourceInterfacePtr & _other );
        void addEvent( const SourceInterfacePtr & _source, const EventInterfacePtr & _event );
        void addSemaphore( const SourceInterfacePtr & _source, const SemaphoreInterfacePtr & _semaphore, uint32_t _flags, int32_t _test, int32_t _apply );
        void addSemaphoreEqual( const SourceInterfacePtr & _source, const SemaphoreInterfacePtr & _semaphore, int32_t _test );
        void addSemaphoreNotEqual( const SourceInterfacePtr & _source, const SemaphoreInterfacePtr & _semaphore, int32_t _test );
        void addSemaphoreGreater( const SourceInterfacePtr & _source, const SemaphoreInterfacePtr & _semaphore, int32_t _test );
        void addSemaphoreLess( const SourceInterfacePtr & _source, const SemaphoreInterfacePtr & _semaphore, int32_t _test );
        void addSemaphoreGreaterEqual( const SourceInterfacePtr & _source, const SemaphoreInterfacePtr & _semaphore, int32_t _test );
        void addSemaphoreLessEqual( const SourceInterfacePtr & _source, const SemaphoreInterfacePtr & _semaphore, int32_t _test );
        void addSemaphoreAssign( const SourceInterfacePtr & _source, const SemaphoreInterfacePtr & _semaphore, int32_t _apply );
        void addSemaphoreAdd( const SourceInterfacePtr & _source, const SemaphoreInterfacePtr & _semaphore, int32_t _apply );
        void addSemaphoreSubtract( const SourceInterfacePtr & _source, const SemaphoreInterfacePtr & _semaphore, int32_t _apply );
        void addFunctionProvider( const SourceInterfacePtr & _source, const FunctionProviderInterfacePtr & _provider );
        void addFunctionContextProvider( const SourceInterfacePtr & _source, const FunctionContextProviderInterfacePtr & _provider );
        void addCallbackProvider( const SourceInterfacePtr & _source, const CallbackProviderInterfacePtr & _provider );
        void addScopeProvider( const SourceInterfacePtr & _source, const ScopeProviderInterfacePtr & _provider );
        void addWhileProvider( const SourceInterfacePtr & _source, const WhileProviderInterfacePtr & _provider );
        void addForProvider( const SourceInterfacePtr & _source, const ForProviderInterfacePtr & _provider, uint32_t _iterator, uint32_t _count );
        void addGeneratorProvider( const SourceInterfacePtr & _source, float _time, uint32_t _iterator, const TimerInterfacePtr & _timer, const GeneratorProviderInterfacePtr & _provider );
        void addTriggerProvider( const SourceInterfacePtr & _source, const EventInterfacePtr & _event, const TriggerProviderInterfacePtr & _provider );
        ArraySources<2> addIfProvider( const SourceInterfacePtr & _source, const IfProviderInterfacePtr & _provider );
        void addTaskFork( const SourceInterfacePtr & _source, const SourceInterfacePtr & _other );
        void addTaskGuard( const SourceInterfacePtr & _source, const GuardProviderInterfacePtr & _begin, const GuardProviderInterfacePtr & _end );
        void addTaskIf( const SourceInterfacePtr & _source, const IfProviderInterfacePtr & _provider, const SourceInterfacePtr & _sourceTrue, const SourceInterfacePtr & _sourceFalse );
        void addTaskRepeat( const SourceInterfacePtr & _source, const WhileProviderInterfacePtr & _provider, const SourceInterfacePtr & _sourceUntil );
        ViewSources<class SourceInterface> addParallel( const SourceInterfacePtr & _source, uint32_t _count );
        ViewSources<SourceInterface> addRace( const SourceInterfacePtr & _source, uint32_t _count );
        ArrayTypeSources<SourceInterface, 2> addIf( const SourceInterfacePtr & _source, bool * _member );
        SourceInterfacePtr addFork( const SourceInterfacePtr & _source );
        const VectorSources & addTaskSwitch( const SourceInterfacePtr & _source, uint32_t _count, const SwitchProviderInterfacePtr & _provider );
        //////////////////////////////////////////////////////////////////////////
        template<class T, class ... Args>
        void addTask( const SourceInterfacePtr & _source, Args && ... _args )
        {
            Allocator * allocator = _source->getAllocator();

            TaskInterfacePtr provider = Helper::makeTask<T>( allocator, std::forward<Args>( _args ) ... );

            NodeInterfacePtr node = _source->makeNode( provider );

            _source->addNode( node );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class T, class ... Args>
        IntrusivePtr<T> emplaceTask( const SourceInterfacePtr & _source, Args && ... _args )
        {
            Allocator * allocator = _source->getAllocator();

            IntrusivePtr<T> provider = Helper::makeTask<T>( allocator, std::forward<Args>( _args ) ... );

            NodeInterfacePtr node = _source->makeNode( provider );

            _source->addNode( node );

            return provider;
        }
        //////////////////////////////////////////////////////////////////////////
        template<class F>
        void addFunction( const SourceInterfacePtr & _source, F _f )
        {
            Allocator * allocator = _source->getAllocator();

            FunctionProviderInterfacePtr provider = Helper::makeFunctionProvider( allocator, _f );

            Cook::addFunctionProvider( _source, provider );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class C, class M, class ... Args>
        void addFunction( const SourceInterfacePtr & _source, C * _self, M _method, Args && ... _args )
        {
            Allocator * allocator = _source->getAllocator();

            FunctionProviderInterfacePtr provider = Helper::makeFunctionProvider( allocator, [&, _self, _method, _args ...]()
            {
                (_self->*_method)(_args ...);
            } );

            Cook::addFunctionProvider( _source, provider );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class F>
        void addFunctionContext( const SourceInterfacePtr & _source, F _f )
        {
            Allocator * allocator = _source->getAllocator();

            FunctionContextProviderInterfacePtr provider = Helper::makeFunctionContextProvider( allocator, _f );

            Cook::addFunctionContextProvider( _source, provider );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class C, class M, class ... Args>
        void addFunctionContext( const SourceInterfacePtr & _source, C * _self, M _method, Args && ... _args )
        {
            Allocator * allocator = _source->getAllocator();

            FunctionContextProviderInterfacePtr provider = Helper::makeFunctionContextProvider( allocator, [&, _self, _method, _args ...]( bool _skip )
            {
                (_self->*_method)(_skip, _args ...);
            } );

            Cook::addFunctionContextProvider( _source, provider );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class F>
        void addCallback( const SourceInterfacePtr & _source, F _f )
        {
            Allocator * allocator = _source->getAllocator();

            CallbackProviderInterfacePtr provider = Helper::makeCallbackProvider( allocator, _f );

            Cook::addCallbackProvider( _source, provider );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class C, class M, class ... Args>
        void addCallback( const SourceInterfacePtr & _source, C * _self, M _method, Args && ... _args )
        {
            Allocator * allocator = _source->getAllocator();

            CallbackProviderInterfacePtr provider = Helper::makeCallbackProvider( allocator, [&, _self, _method, _args ...]( const CallbackObserverInterfacePtr & _callback, bool _skip )
            {
                (_self->*_method)(_callback, _skip, _args ...);
            } );

            Cook::addCallbackProvider( _source, provider );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class F>
        void addScope( const SourceInterfacePtr & _source, F _f )
        {
            Allocator * allocator = _source->getAllocator();

            ScopeProviderInterfacePtr provider = Helper::makeScopeProvider( allocator, _f );

            Cook::addScopeProvider( _source, provider );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class C, class M, class ... Args>
        void addScope( const SourceInterfacePtr & _source, C * _self, M _method, Args && ... _args )
        {
            Allocator * allocator = _source->getAllocator();

            ScopeProviderInterfacePtr provider = Helper::makeScopeProvider( allocator, [&, _self, _method, _args ...]( const SourceInterfacePtr & _source )
            {
                (_self->*_method)(_source, _args ...);
            } );

            Cook::addScopeProvider( _source, provider );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class FD, class FE>
        void addGenerator( const SourceInterfacePtr & _source, const TimerInterfacePtr & _timer, FD _fdelay, FE _fevent )
        {
            Allocator * allocator = _source->getAllocator();

            GeneratorProviderInterfacePtr provider = Helper::makeGeneratorProvider( allocator, _fdelay, _fevent );

            Cook::addGeneratorProvider( _source, 0.f, 0, _timer, provider );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class F>
        void addWhile( const SourceInterfacePtr & _source, F _f )
        {
            Allocator * allocator = _source->getAllocator();

            WhileProviderInterfacePtr provider = Helper::makeWhileProvider<F>( allocator, _f );

            Cook::addWhileProvider( _source, provider );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class C, class M, class ... Args>
        void addWhile( const SourceInterfacePtr & _source, C * _self, M _method, Args && ... _args )
        {
            Allocator * allocator = _source->getAllocator();

            WhileProviderInterfacePtr provider = Helper::makeWhileProvider( allocator, [&, _self, _method, _args ...]( const SourceInterfacePtr & _source ){ return (_self->*_method)(_source, _args ...); } );

            Cook::addWhileProvider( _source, provider );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class F>
        void addFor( const SourceInterfacePtr & _source, uint32_t _count, F _f )
        {
            Allocator * allocator = _source->getAllocator();

            ForProviderInterfacePtr provider = Helper::makeForProvider( allocator, _f );

            Cook::addForProvider( _source, provider, 0, _count );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class C, class M, class ... Args>
        void addFor( const SourceInterfacePtr & _source, uint32_t _count, C * _self, M _method, Args && ... _args )
        {
            Allocator * allocator = _source->getAllocator();

            ForProviderInterfacePtr provider = Helper::makeForProvider( allocator, [&, _self, _method, _args ...]( const SourceInterfacePtr & _source, uint32_t _iterator, uint32_t _count ){ return (_self->*_method)(_source, _iterator, _count, _args ...); } );

            Cook::addForProvider( _source, provider, 0, _count );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class F>
        void addFor( const SourceInterfacePtr & _source, uint32_t _iterator, uint32_t _count, F _f )
        {
            Allocator * allocator = _source->getAllocator();

            ForProviderInterfacePtr provider = Helper::makeForProvider( allocator, _f );

            Cook::addForProvider( _source, provider, _iterator, _count );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class C, class M, class ... Args>
        void addFor( const SourceInterfacePtr & _source, uint32_t _iterator, uint32_t _count, C * _self, M _method, Args && ... _args )
        {
            Allocator * allocator = _source->getAllocator();

            ForProviderInterfacePtr provider = Helper::makeForProvider( allocator, [&, _self, _method, _args ...]( const SourceInterfacePtr & _source, uint32_t _iterator, uint32_t _count ){ return (_self->*_method)(_source, _iterator, _count, _args ...); } );

            Cook::addForProvider( _source, provider, _iterator, _count );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class T, class ... Args>
        ArraySources<2> tryTask( const SourceInterfacePtr & _source, Args && ... _args )
        {
            Allocator * allocator = _source->getAllocator();

            TaskInterfacePtr provider = Helper::makeTask<T>( allocator, std::forward<Args>( _args ) ... );

            NodeInterfacePtr node = _source->makeNode( provider );

            _source->addNode( node );

            ArraySources<2> desc = Cook::addIf( _source, [node]()
            {
                if( node->isError() == true )
                {
                    return false;
                }

                return true;
            } );

            return desc;
        }
        //////////////////////////////////////////////////////////////////////////
        template<size_t Count>
        ArrayTypeSources<SourceInterface, Count> addParallel( const SourceInterfacePtr & _source )
        {
            ArraySources<Count> sources;

            for( SourceInterfacePtr & source : sources )
            {
                source = _source->makeSource();
            }

            Allocator * allocator = _source->getAllocator();

            TranscriptorParallelArrayPtr<Count> transcriptor = Helper::makeTranscriptorParallelArray( allocator, std::move( sources ) );

            const SourceProviderInterfacePtr & provider = _source->getSourceProvider();

            provider->addTranscriptor( transcriptor );

            const ArraySources<Count> & transcriptor_sources = transcriptor->getSources();

            return Helper::ArraySourcesCast<SourceInterface>( transcriptor_sources );
        }
        //////////////////////////////////////////////////////////////////////////
        template<size_t Count>
        ArraySources<Count> addRace( const SourceInterfacePtr & _source )
        {
            ArraySources<Count> sources;

            for( SourceInterfacePtr & source : sources )
            {
                source = _source->makeSource();
            }

            Allocator * allocator = _source->getAllocator();

            TranscriptorRaceArrayPtr<Count> transcriptor = Helper::makeTranscriptorRaceArray( allocator, std::move( sources ) );

            const SourceProviderInterfacePtr & provider = _source->getSourceProvider();

            provider->addTranscriptor( transcriptor );

            const ArraySources<Count> & transcriptor_sources = transcriptor->getSources();

            return Helper::ArraySourcesCast<SourceInterface>( transcriptor_sources );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class F>
        void addTrigger( const SourceInterfacePtr & _source, const EventInterfacePtr & _event, F _f )
        {
            Allocator * allocator = _source->getAllocator();

            TriggerProviderInterfacePtr provider = Helper::makeTriggerProvider( allocator, _f );

            Cook::addTriggerProvider( _source, _event, provider );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class F>
        ArrayTypeSources<SourceInterface, 2> addIf( const SourceInterfacePtr & _source, F _f )
        {
            Allocator * allocator = _source->getAllocator();

            IfProviderInterfacePtr provider = Helper::makeIfProvider( allocator, _f );

            ArraySources<2> sources = Cook::addIfProvider( _source, provider );

            return Helper::ArraySourcesCast<SourceInterface>( sources );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class C, class M, class ... Args>
        ArrayTypeSources<SourceInterface, 2> addIf( const SourceInterfacePtr & _source, C * _self, M _method, Args && ... _args )
        {
            Allocator * allocator = _source->getAllocator();

            IfProviderInterfacePtr provider = Helper::makeIfProvider( allocator, [&, _self, _method, _args ...]()
            {
                return (_self->*_method)(_args ...);
            } );

            ArraySources<2> sources = Cook::addIfProvider( _source, provider );

            return Helper::ArraySourcesCast<SourceInterface>( sources );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class F>
        SourceInterfacePtr addIfSuccessful( const SourceInterfacePtr & _source, F _f )
        {
            Allocator * allocator = _source->getAllocator();

            IfProviderInterfacePtr provider = Helper::makeIfProvider( allocator, _f );

            ArraySources<2> sources = Cook::addIfProvider( _source, provider );

            return sources[0];
        }
        //////////////////////////////////////////////////////////////////////////
        template<class C, class M, class ... Args>
        SourceInterfacePtr addIfSuccessful( const SourceInterfacePtr & _source, C * _self, M _method, Args && ... _args )
        {
            Allocator * allocator = _source->getAllocator();

            IfProviderInterfacePtr provider = Helper::makeIfProvider( allocator, [&, _self, _method, _args ...]()
            {
                return (_self->*_method)(_args ...);
            } );

            ArraySources<2> sources = Cook::addIfProvider( _source, provider );

            return sources[0];
        }
        //////////////////////////////////////////////////////////////////////////
        template<class F>
        SourceInterfacePtr addIfFailure( const SourceInterfacePtr & _source, F _f )
        {
            Allocator * allocator = _source->getAllocator();

            IfProviderInterfacePtr provider = Helper::makeIfProvider( allocator, _f );

            ArraySources<2> sources = Cook::addIfProvider( _source, provider );

            return sources[1];
        }
        //////////////////////////////////////////////////////////////////////////
        template<class C, class M, class ... Args>
        SourceInterfacePtr addIfFailure( const SourceInterfacePtr & _source, C * _self, M _method, Args && ... _args )
        {
            Allocator * allocator = _source->getAllocator();

            IfProviderInterfacePtr provider = Helper::makeIfProvider( allocator, [&, _self, _method, _args ...]()
            {
                return (_self->*_method)(_args ...);
            } );

            ArraySources<2> sources = Cook::addIfProvider( _source, provider );

            return sources[1];
        }
        //////////////////////////////////////////////////////////////////////////
        template<class F>
        ArrayTypeSources<SourceInterface, 2> addUnless( const SourceInterfacePtr & _source, F _f )
        {
            Allocator * allocator = _source->getAllocator();

            IfProviderInterfacePtr provider = Helper::makeIfProvider( allocator, _f );

            ArraySources<2> sources = Cook::addIfProvider( _source, provider );

            return ArrayTypeSources<SourceInterface, 2>{
                {
                    sources[1], sources[0]
                }};
        }
        //////////////////////////////////////////////////////////////////////////
        template<class C, class M, class ... Args>
        ArrayTypeSources<SourceInterface, 2> addUnless( const SourceInterfacePtr & _source, C * _self, M _method, Args && ... _args )
        {
            Allocator * allocator = _source->getAllocator();

            IfProviderInterfacePtr provider = Helper::makeIfProvider( allocator, [&, _self, _method, _args ...]()
            {
                return (_self->*_method)(_args ...);
            } );

            ArraySources<2> sources = Cook::addIfProvider( _source, provider );

            return ArrayTypeSources<SourceInterface, 2>{
                {
                    sources[1], sources[0]
                }};
        }
        //////////////////////////////////////////////////////////////////////////
        template<class F>
        ViewSources<SourceInterface> addSwitch( const SourceInterfacePtr & _source, uint32_t _count, F _f )
        {
            Allocator * allocator = _source->getAllocator();

            SwitchProviderInterfacePtr provider = Helper::makeSwitchProvider( allocator, _f );

            const VectorSources & sources_switch = Cook::addTaskSwitch( _source, _count, provider );

            return ViewSources<SourceInterface>( sources_switch );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class F>
        SourceInterfacePtr addRepeat( const SourceInterfacePtr & _source, F _f )
        {
            Allocator * allocator = _source->getAllocator();

            WhileProviderInterfacePtr provider = Helper::makeWhileProvider( allocator, _f );

            SourceInterfacePtr source_until = _source->makeSource();

            Cook::addTaskRepeat( _source, provider, source_until );

            return source_until;
        }
        //////////////////////////////////////////////////////////////////////////
        template<class C, class M, class ... Args>
        SourceInterfacePtr addRepeat( const SourceInterfacePtr & _source, C * _self, M _method, Args && ... _args )
        {
            Allocator * allocator = _source->getAllocator();

            WhileProviderInterfacePtr provider = Helper::makeWhileProvider( allocator, [&, _self, _method, _args ...]( const SourceInterfacePtr & _source )
            {
                return (_self->*_method)(_source, _args ...);
            } );

            SourceInterfacePtr source_until = _source->makeSource();

            Cook::addTaskRepeat( _source, provider, source_until );

            return source_until;
        }
        //////////////////////////////////////////////////////////////////////////
        template<class FB, class FE>
        SourceInterfacePtr addGuard( const SourceInterfacePtr & _source, FB _begin, FE _end )
        {
            Allocator * allocator = _source->getAllocator();

            GuardProviderInterfacePtr begin_provider = Helper::makeGuardProvider( allocator, _begin );
            GuardProviderInterfacePtr end_provider = Helper::makeGuardProvider( allocator, _end );

            auto && [source_guard, source_code] = Cook::addRace<2>( _source );

            Cook::addTaskGuard( _source, begin_provider, end_provider );

            return source_code;
        }
        //////////////////////////////////////////////////////////////////////////
        template<class C>
        Zip<SourceInterfacePtr, typename ViewSources<SourceInterface>::iterator, typename C::iterator> addParallelZip( const SourceInterfacePtr & _source, C & _c )
        {
            typename C::size_type zip_size = _c.size();
            ViewSources<SourceInterface> parallel_sources = addParallel( _source, (uint32_t)zip_size );

            return Helper::makeZip<SourceInterfacePtr>( parallel_sources.begin(), parallel_sources.end(), _c.begin(), _c.end() );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class C>
        Zip<SourceInterfacePtr, typename ViewSources<SourceInterface>::iterator, typename C::const_iterator> addParallelZip( const SourceInterfacePtr & _source, const C & _c )
        {
            typename C::size_type zip_size = _c.size();
            ViewSources<SourceInterface> parallel_sources = addParallel( _source, (uint32_t)zip_size );

            return Helper::makeZip<SourceInterfacePtr>( parallel_sources.begin(), parallel_sources.end(), _c.begin(), _c.end() );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class C>
        Zip<SourceInterfacePtr, typename ViewSources<SourceInterface>::iterator, typename C::iterator> addRaceZip( const SourceInterfacePtr & _source, C & _c )
        {
            typename C::size_type zip_size = _c.size();
            ViewSources<SourceInterface> parallel_sources = addRace( _source, (uint32_t)zip_size );

            return Helper::makeZip<SourceInterfacePtr>( parallel_sources.begin(), parallel_sources.end(), _c.begin(), _c.end() );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class C>
        Zip<SourceInterfacePtr, typename ViewSources<SourceInterface>::iterator, typename C::const_iterator> addRaceZip( const SourceInterfacePtr & _source, const C & _c )
        {
            typename C::size_type zip_size = _c.size();
            ViewSources<SourceInterface> parallel_sources = addRace( _source, (uint32_t)zip_size );

            return Helper::makeZip<SourceInterfacePtr>( parallel_sources.begin(), parallel_sources.end(), _c.begin(), _c.end() );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class C, class F>
        Zip<SourceInterfacePtr, typename ViewSources<SourceInterface>::iterator, typename C::iterator> addSwitchZip( const SourceInterfacePtr & _source, C & _c, F _f )
        {
            typename C::size_type zip_size = _c.size();
            ViewSources<SourceInterface> parallel_sources = addSwitch( _source, (uint32_t)zip_size, _f );

            return Helper::makeZip<SourceInterfacePtr>( parallel_sources.begin(), parallel_sources.end(), _c.begin(), _c.end() );
        }
        //////////////////////////////////////////////////////////////////////////
        template<class C, class F>
        Zip<SourceInterfacePtr, typename ViewSources<SourceInterface>::iterator, typename C::const_iterator> addSwitchZip( const SourceInterfacePtr & _source, const C & _c, F _f )
        {
            typename C::size_type zip_size = _c.size();
            ViewSources<SourceInterface> parallel_sources = addSwitch( _source, (uint32_t)zip_size, _f );

            return Helper::makeZip<SourceInterfacePtr>( parallel_sources.begin(), parallel_sources.end(), _c.begin(), _c.end() );
        }
        //////////////////////////////////////////////////////////////////////////
    }
}