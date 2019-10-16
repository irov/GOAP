/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Factorable.h"
#include "GOAP/IntrusivePtr.h"
#include "GOAP/Vector.h"
#include "GOAP/Zip.h"
#include "GOAP/Task.h"
#include "GOAP/Timer.h"

#include "GOAP/FunctionProvider.h"
#include "GOAP/FunctionContextProvider.h"
#include "GOAP/CallbackProvider.h"
#include "GOAP/ScopeProvider.h"
#include "GOAP/IfProvider.h"
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
    typedef IntrusivePtr<class Task> TaskPtr;
    typedef IntrusivePtr<class Chain> ChainPtr;
    typedef IntrusivePtr<class Source> SourcePtr;
    typedef IntrusivePtr<class Transcriptor> TranscriptorPtr;
    typedef IntrusivePtr<class Event> EventPtr;
    typedef IntrusivePtr<class Semaphore> SemaphorePtr;
    //////////////////////////////////////////////////////////////////////////
    typedef Vector<SourcePtr> VectorSources;
    typedef Vector<TranscriptorPtr> VectorTranscriptor;
    //////////////////////////////////////////////////////////////////////////
    class Source
        : public Factorable
    {
    public:
        Source();
        ~Source() override;

    public:
        void setSkip( bool _skip );
        bool isSkip() const;

    public:
        bool empty() const;

    public:
        void addTask( const TaskPtr & _task );

        template<class T, class ... Args>
        void addTask( Args && ... _args )
        {
            TaskPtr task = Helper::makeTask<T>( _args ... );

            this->addTask( task );
        }

        template<class T, class ... Args>
        IntrusivePtr<T> emplaceTask( Args && ... _args )
        {
            IntrusivePtr<T> task = Helper::makeTask<T>( std::forward<Args &&>( _args ) ... );

            this->addTask( task );

            return task;
        }

    public:
        ArraySources<2> tryTask( const TaskPtr & _task );

    public:
        const VectorSources & addParallel( uint32_t _count );

        template<size_t Count>
        const ArraySources<Count> & addParallel()
        {
            ArraySources<Count> sources;

            for( SourcePtr & source : sources )
            {
                source = this->_provideSource();
            }

            TranscriptorParallelArrayPtr<Count> transcriptor = Helper::makeTranscriptorParallelArray( std::move( sources ) );

            m_transcriptors.push_back( transcriptor );

            const ArraySources<Count> & transcriptor_sources = transcriptor->getSources();

            return transcriptor_sources;
        }

        const VectorSources & addRace( uint32_t _count );

        template<size_t Count>
        const ArraySources<Count> & addRace()
        {
            ArraySources<Count> sources;

            for( SourcePtr & source : sources )
            {
                source = this->_provideSource();
            }

            TranscriptorRaceArrayPtr<Count> transcriptor = Helper::makeTranscriptorRaceArray( std::move( sources ) );

            m_transcriptors.push_back( transcriptor );

            const ArraySources<Count> & transcriptor_sources = transcriptor->getSources();

            return transcriptor_sources;
        }

        SourcePtr addFork();
        void addBlock();
        void addNoSkip();

        void addSource( const SourcePtr & _source );

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
            ScopeProviderPtr provider = Helper::makeScopeProvider( [&, _self, _method, _args ...]( const SourcePtr & _source ){ (_self->*_method)(_source, _args ...); } );

            this->addScopeProvider( provider );
        }

        template<class F>
        ArraySources<2> addIf( F _f )
        {
            IfProviderPtr provider = Helper::makeIfProvider( _f );

            ArraySources<2> desc = this->addIfProvider( provider );

            return desc;
        }

        template<class C, class M, class ... Args>
        ArraySources<2> addIf( C * _self, M _method, Args && ... _args )
        {
            IfProviderPtr provider = Helper::makeIfProvider( [&, _self, _method, _args ...](){ return (_self->*_method)(_args ...); } );

            ArraySources<2> desc = this->addIfProvider( provider );

            return desc;
        }

        template<class F>
        SourcePtr addIfSuccessful( F _f )
        {
            IfProviderPtr provider = Helper::makeIfProvider( _f );

            ArraySources<2> desc = this->addIfProvider( provider );

            return desc[0];
        }

        template<class C, class M, class ... Args>
        SourcePtr addIfSuccessful( C * _self, M _method, Args && ... _args )
        {
            IfProviderPtr provider = Helper::makeIfProvider( [&, _self, _method, _args ...](){ return (_self->*_method)(_args ...); } );

            ArraySources<2> desc = this->addIfProvider( provider );

            return desc[0];
        }

        template<class F>
        SourcePtr addIfFailure( F _f )
        {
            IfProviderPtr provider = Helper::makeIfProvider( _f );

            ArraySources<2> desc = this->addIfProvider( provider );

            return desc[1];
        }

        template<class C, class M, class ... Args>
        SourcePtr addIfFailure( C * _self, M _method, Args && ... _args )
        {
            IfProviderPtr provider = Helper::makeIfProvider( [&, _self, _method, _args ...](){ return (_self->*_method)(_args ...); } );

            ArraySources<2> desc = this->addIfProvider( provider );

            return desc[1];
        }

        template<class F>
        ArraySources<2> addUnless( F _f )
        {
            IfProviderPtr provider = Helper::makeIfProvider( _f );

            ArraySources<2> desc = this->addUnlessProvider( provider );

            return desc;
        }

        template<class C, class M, class ... Args>
        ArraySources<2> addUnless( C * _self, M _method, Args && ... _args )
        {
            IfProviderPtr provider = Helper::makeIfProvider( [&, _self, _method, _args ...](){ return (_self->*_method)(_args ...); } );

            ArraySources<2> desc = this->addUnlessProvider( provider );

            return desc;
        }

        template<class F>
        const VectorSources & addSwitch( uint32_t _count, F _f )
        {
            SwitchProviderPtr provider = Helper::makeSwitchProvider( _f );

            const VectorSources & sources = this->addSwitchProvider( provider, _count );

            return sources;
        }

        template<size_t Count, class F>
        const VectorSources & addSwitch( uint32_t _count, F _f )
        {
            SwitchProviderPtr provider = Helper::makeSwitchProvider( _f );

            const VectorSources & sources = this->addSwitchProvider( provider, _count );

            return sources;
        }

        template<class FD, class FE>
        void addGenerator( const TimerPtr & _timer, FD _fdelay, FE _fevent )
        {
            GeneratorProviderPtr provider = Helper::makeGeneratorProvider( _fdelay, _fevent );

            this->addGeneratorProvider( 0.f, 0, _timer, provider );
        }

        template<class F>
        SourcePtr addRepeat( F _f )
        {
            WhileProviderPtr provider = Helper::makeWhileProvider( _f );

            SourcePtr source = this->addRepeatProvider( provider );

            return source;
        }

        template<class C, class M, class ... Args>
        SourcePtr addRepeat( C * _self, M _method, Args && ... _args )
        {
            WhileProviderPtr provider = Helper::makeWhileProvider( [&, _self, _method, _args ...]( const SourcePtr & _source ){ return (_self->*_method)(_source, _args ...); } );

            SourcePtr source = this->addRepeatProvider( provider );

            return source;
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
            WhileProviderPtr provider = Helper::makeWhileProvider( [&, _self, _method, _args ...]( const SourcePtr & _source ){ return (_self->*_method)(_source, _args ...); } );

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
            ForProviderPtr provider = Helper::makeForProvider( [&, _self, _method, _args ...]( const SourcePtr & _source, uint32_t _iterator, uint32_t _count ){ return (_self->*_method)(_source, _iterator, _count, _args ...); } );

            this->addForProvider( provider, 0, _count );
        }

        template<class FB, class FE>
        SourcePtr addGuard( FB _begin, FE _end )
        {
            GuardProviderPtr begin_provider = Helper::makeGuardProvider( _begin );
            GuardProviderPtr end_provider = Helper::makeGuardProvider( _end );

            SourcePtr source = this->addGuardProvider( begin_provider, end_provider );

            return source;
        }
        
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
        template<class C>
        Zip<VectorSources::const_iterator, typename C::iterator> addParallelZip( C & _c )
        {
            typename C::size_type zip_size = _c.size();
            const VectorSources & parallel_sources = this->addParallel( (uint32_t)zip_size );

            return Zip<VectorSources::const_iterator, typename C::iterator>( parallel_sources.begin(), parallel_sources.end(), _c.begin(), _c.end() );
        }

        template<class C>
        Zip<VectorSources::const_iterator, typename C::const_iterator> addParallelZip( const C & _c )
        {
            typename C::size_type zip_size = _c.size();
            const VectorSources & parallel_sources = this->addParallel( (uint32_t)zip_size );

            return Zip<VectorSources::const_iterator, typename C::const_iterator>( parallel_sources.begin(), parallel_sources.end(), _c.begin(), _c.end() );
        }

        template<class C>
        Zip<VectorSources::const_iterator, typename C::iterator> addRaceZip( C & _c )
        {
            typename C::size_type zip_size = _c.size();
            const VectorSources & parallel_sources = this->addRace( (uint32_t)zip_size );

            return Zip<VectorSources::const_iterator, typename C::iterator>( parallel_sources.begin(), parallel_sources.end(), _c.begin(), _c.end() );
        }

        template<class C>
        Zip<VectorSources::const_iterator, typename C::const_iterator> addRaceZip( const C & _c )
        {
            typename C::size_type zip_size = _c.size();
            const VectorSources & parallel_sources = this->addRace( (uint32_t)zip_size );

            return Zip<VectorSources::const_iterator, typename C::const_iterator>( parallel_sources.begin(), parallel_sources.end(), _c.begin(), _c.end() );
        }

        template<class C, class F>
        Zip<VectorSources::const_iterator, typename C::iterator> addSwitchZip( C & _c, F _f )
        {
            typename C::size_type zip_size = _c.size();
            const VectorSources & parallel_sources = this->addSwitch( (uint32_t)zip_size, _f );

            return Zip<VectorSources::const_iterator, typename C::iterator>( parallel_sources.begin(), parallel_sources.end(), _c.begin(), _c.end() );
        }

        template<class C, class F>
        Zip<VectorSources::const_iterator, typename C::const_iterator> addSwitchZip( const C & _c, F _f )
        {
            typename C::size_type zip_size = _c.size();
            const VectorSources & parallel_sources = this->addSwitch( (uint32_t)zip_size, _f );

            return Zip<VectorSources::const_iterator, typename C::const_iterator>( parallel_sources.begin(), parallel_sources.end(), _c.begin(), _c.end() );
        }

    public:
        void addFunctionProvider( const FunctionProviderPtr & _provider );
        void addFunctionContextProvider( const FunctionContextProviderPtr & _provider );
        void addCallbackProvider( const CallbackProviderPtr & _provider );
        void addScopeProvider( const ScopeProviderPtr & _provider );
        ArraySources<2> addIfProvider( const IfProviderPtr & _provider );
        ArraySources<2> addUnlessProvider( const IfProviderPtr & _provider );
        SourcePtr addRepeatProvider( const WhileProviderPtr & _provider );
        const VectorSources & addSwitchProvider( const SwitchProviderPtr & _provider, uint32_t _count );
        SourcePtr addGuardProvider( const GuardProviderPtr & _begin, const GuardProviderPtr & _end );
        void addWhileProvider( const WhileProviderPtr & _provider );
        void addForProvider( const ForProviderPtr & _provider, uint32_t _iterator, uint32_t _count );
        void addGeneratorProvider( float _time, uint32_t _iterator, const TimerPtr & _timer, const GeneratorProviderPtr & _provider );

    public:
        TaskPtr parse( const ChainPtr & _chain, const TaskPtr & _task );

    protected:
        virtual SourcePtr _provideSource();

    protected:
        void makeSources_( VectorSources & _sources, uint32_t _count );

    protected:
        VectorTranscriptor m_transcriptors;

        bool m_skip;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<Source> SourcePtr;
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        inline SourcePtr makeSource()
        {
            return SourcePtr::from( new Source() );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}