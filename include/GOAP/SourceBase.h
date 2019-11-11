/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/SourceInterface.h"
#include "GOAP/SourceProviderInterface.h"

#include "GOAP/Vector.h"
#include "GOAP/Zip.h"
#include "GOAP/Node.h"
#include "GOAP/Timer.h"

#include "GOAP/FunctionProvider.h"
#include "GOAP/FunctionContextProvider.h"
#include "GOAP/CallbackProvider.h"
#include "GOAP/ScopeProvider.h"
#include "GOAP/IfProvider.h"
#include "GOAP/IfProviderMember.h"
#include "GOAP/TriggerProvider.h"
#include "GOAP/SwitchProvider.h"
#include "GOAP/GuardProvider.h"
#include "GOAP/ForProvider.h"
#include "GOAP/GeneratorProvider.h"
#include "GOAP/SemaphoreFlags.h"
#include "GOAP/WhileProvider.h"
#include "GOAP/TranscriptorParallelArray.h"
#include "GOAP/TranscriptorRaceArray.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class Chain> ChainPtr;
    typedef IntrusivePtr<class TranscriptorInterface> TranscriptorInterfacePtr;
    typedef IntrusivePtr<class Event> EventPtr;
    typedef IntrusivePtr<class Semaphore> SemaphorePtr;
    //////////////////////////////////////////////////////////////////////////
    class SourceBase
        : public SourceInterface
    {
    public:
        typedef Vector<SourceInterfacePtr> VectorSources;

    public:
        SourceBase( const SourceProviderInterfacePtr & _provider );
        ~SourceBase() override;

    public:
        const SourceProviderInterfacePtr & getSourceProvider() const override;

    public:
        SourceInterfacePtr makeSource() override;

    protected:
        virtual SourceInterfacePtr _makeSource() = 0;

    public:
        NodeInterfacePtr makeNode( const TaskInterfacePtr & _provider ) override;
        void addNode( const NodeInterfacePtr & _task ) override;

    public:
        template<class T, class ... Args>
        void addTask( Args && ... _args )
        {
            TaskInterfacePtr provider = Helper::makeTask<T>( std::forward<Args &&>( _args ) ... );

            NodePtr node = this->makeNode( provider );

            this->addNode( node );
        }

        template<class T, class ... Args>
        IntrusivePtr<T> emplaceTask( Args && ... _args )
        {
            IntrusivePtr<T> provider = Helper::makeTask<T>( std::forward<Args &&>( _args ) ... );

            NodePtr node = this->makeNode( provider );

            this->addNode( node );

            return provider;
        }

    public:
        void addBlock();
        void addNoSkip();

        template<class F>
        void addFunction( F _f )
        {
            FunctionProviderPtr provider = Helper::makeFunctionProvider( _f );

            this->addFunctionProvider( provider );
        }

        template<class C, class M, class ... Args>
        void addFunction( C * _self, M _method, Args && ... _args )
        {
            FunctionProviderPtr provider = Helper::makeFunctionProvider( [&, _self, _method, _args ...](){ (_self->*_method)(_args ...); } );

            this->addFunctionProvider( provider );
        }

        template<class F>
        void addFunctionContext( F _f )
        {
            FunctionContextProviderPtr provider = Helper::makeFunctionContextProvider( _f );

            this->addFunctionContextProvider( provider );
        }

        template<class C, class M, class ... Args>
        void addFunctionContext( C * _self, M _method, Args && ... _args )
        {
            FunctionContextProviderPtr provider = Helper::makeFunctionContextProvider( [&, _self, _method, _args ...]( bool _skip ){ (_self->*_method)(_skip, _args ...); } );

            this->addFunctionContextProvider( provider );
        }

        template<class F>
        void addCallback( F _f )
        {
            CallbackProviderPtr provider = Helper::makeCallbackProvider( _f );

            this->addCallbackProvider( provider );
        }

        template<class C, class M, class ... Args>
        void addCallback( C * _self, M _method, Args && ... _args )
        {
            CallbackProviderPtr provider = Helper::makeCallbackProvider( [&, _self, _method, _args ...]( const CallbackObserverPtr & _callback, bool _skip ){ (_self->*_method)(_callback, _skip, _args ...); } );

            this->addCallbackProvider( provider );
        }

        template<class F>
        void addScope( F _f )
        {
            ScopeProviderPtr provider = Helper::makeScopeProvider( _f );

            this->addScopeProvider( provider );
        }

        template<class C, class M, class ... Args>
        void addScope( C * _self, M _method, Args && ... _args )
        {
            ScopeProviderPtr provider = Helper::makeScopeProvider( [&, _self, _method, _args ...]( const SourceInterfacePtr & _source ){ (_self->*_method)(_source, _args ...); } );

            this->addScopeProvider( provider );
        }

        template<class FD, class FE>
        void addGenerator( const TimerPtr & _timer, FD _fdelay, FE _fevent )
        {
            GeneratorProviderPtr provider = Helper::makeGeneratorProvider( _fdelay, _fevent );

            this->addGeneratorProvider( 0.f, 0, _timer, provider );
        }

        template<class F>
        void addWhile( F _f )
        {
            WhileProviderPtr provider = Helper::makeWhileProvider<F>( _f );

            this->addWhileProvider( provider );
        }

        template<class C, class M, class ... Args>
        void addWhile( C * _self, M _method, Args && ... _args )
        {
            WhileProviderPtr provider = Helper::makeWhileProvider( [&, _self, _method, _args ...]( const SourceInterfacePtr & _source ){ return (_self->*_method)(_source, _args ...); } );

            this->addWhileProvider( provider );
        }

        template<class F>
        void addFor( uint32_t _count, F _f )
        {
            ForProviderPtr provider = Helper::makeForProvider( _f );

            this->addForProvider( provider, 0, _count );
        }

        template<class C, class M, class ... Args>
        void addFor( uint32_t _count, C * _self, M _method, Args && ... _args )
        {
            ForProviderPtr provider = Helper::makeForProvider( [&, _self, _method, _args ...]( const SourceInterfacePtr & _source, uint32_t _iterator, uint32_t _count ){ return (_self->*_method)(_source, _iterator, _count, _args ...); } );

            this->addForProvider( provider, 0, _count );
        }

        template<class F>
        void addFor( uint32_t _iterator, uint32_t _count, F _f )
        {
            ForProviderPtr provider = Helper::makeForProvider( _f );

            this->addForProvider( provider, _iterator, _count );
        }

        template<class C, class M, class ... Args>
        void addFor( uint32_t _iterator, uint32_t _count, C * _self, M _method, Args && ... _args )
        {
            ForProviderPtr provider = Helper::makeForProvider( [&, _self, _method, _args ...]( const SourceInterfacePtr & _source, uint32_t _iterator, uint32_t _count ){ return (_self->*_method)(_source, _iterator, _count, _args ...); } );

            this->addForProvider( provider, _iterator, _count );
        }

    public:
        void addSource( const SourceInterfacePtr & _source );
        void addEvent( const EventPtr & _event );

        void addSemaphore( const SemaphorePtr & _semaphore, uint32_t _flags, int32_t _test, int32_t _apply );

        void addSemaphoreEqual( const SemaphorePtr & _semaphore, int32_t _test );
        void addSemaphoreNotEqual( const SemaphorePtr & _semaphore, int32_t _test );
        void addSemaphoreGreater( const SemaphorePtr & _semaphore, int32_t _test );
        void addSemaphoreLess( const SemaphorePtr & _semaphore, int32_t _test );
        void addSemaphoreGreaterEqual( const SemaphorePtr & _semaphore, int32_t _test );
        void addSemaphoreLessEqual( const SemaphorePtr & _semaphore, int32_t _test );

        void addSemaphoreAssign( const SemaphorePtr & _semaphore, int32_t _apply );
        void addSemaphoreAdd( const SemaphorePtr & _semaphore, int32_t _apply );
        void addSemaphoreSubtract( const SemaphorePtr & _semaphore, int32_t _apply );

    public:
        void addFunctionProvider( const FunctionProviderPtr & _provider );
        void addFunctionContextProvider( const FunctionContextProviderPtr & _provider );
        void addCallbackProvider( const CallbackProviderPtr & _provider );
        void addScopeProvider( const ScopeProviderPtr & _provider );
        void addWhileProvider( const WhileProviderPtr & _provider );
        void addForProvider( const ForProviderPtr & _provider, uint32_t _iterator, uint32_t _count );
        void addGeneratorProvider( float _time, uint32_t _iterator, const TimerPtr & _timer, const GeneratorProviderPtr & _provider );
        void addTriggerProvider( const EventPtr & _event, const TriggerProviderPtr & _provider );
        ArraySources<2> addIfProvider( const IfProviderPtr & _provider );

    public:
        void addTaskFork( const SourceInterfacePtr & _source );
        void addTaskGuard( const GuardProviderPtr & _begin, const GuardProviderPtr & _end );
        void addTaskIf( const IfProviderPtr & _provider, const SourceInterfacePtr & _sourceTrue, const SourceInterfacePtr & _sourceFalse );
        void addTaskRepeat( const WhileProviderPtr & _provider, const SourceInterfacePtr & _sourceUntil );

    public:
        const VectorSources & addParallelTranscriptor( uint32_t _count );
        const VectorSources & addRaceTranscriptor( uint32_t _count );
        const VectorSources & addTaskSwitch( uint32_t _count, const SwitchProviderPtr & _provider );

    protected:
        void makeSources_( VectorSources & _sources, uint32_t _count );

    protected:
        SourceProviderInterfacePtr m_provider;
    };
}