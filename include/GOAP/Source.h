/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	pragma once

#	include "GOAP/Config.h"
#	include "GOAP/Vector.h"

#	include "GOAP/FunctionProvider.h"
#	include "GOAP/CallbackProvider.h"
#	include "GOAP/ScopeProvider.h"
#	include "GOAP/IfProvider.h"
#	include "GOAP/SwitchProvider.h"
#	include "GOAP/GuardProvider.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class Task> TaskPtr;
    typedef IntrusivePtr<class Chain> ChainPtr;
    typedef IntrusivePtr<class Source> SourcePtr;
    typedef IntrusivePtr<class Transcriptor> TranscriptorPtr;
    typedef IntrusivePtr<class Semaphore> SemaphorePtr;
    //////////////////////////////////////////////////////////////////////////
    typedef Vector<SourcePtr> VectorSources;
    typedef Vector<TranscriptorPtr> VectorTranscriptor;
    //////////////////////////////////////////////////////////////////////////
    struct IfSource
    {
        SourcePtr source_true;
        SourcePtr source_false;
    };
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
        void addTask( const TaskPtr & _task );

    public:
        const VectorSources & addParallel( size_t _count );
        const VectorSources & addRace( size_t _count );
        SourcePtr addFork();
        void addDeadLock();

        template<class F>
        void addFunction( F _f )
        {
            FunctionProviderPtr provider = Helper::makeFunctionProvider( _f );

            this->addFunctionProvider( provider );
        }

        template<class F>
        void addCallback( F _f )
        {
            CallbackProviderPtr provider = Helper::makeCallbackProvider( _f );

            this->addCallbackProvider( provider );
        }

        template<class F>
        void addScope( F _f )
        {
            ScopeProviderPtr provider = Helper::makeScopeProvider( _f );

            this->addScopeProvider( provider );
        }

        template<class F>
        IfSource addIf( F _f )
        {
            IfProviderPtr provider = Helper::makeIfProvider( _f );

            IfSource desc = this->addIfProvider( provider );

            return desc;
        }

        template<class F>
        IfSource addUnless( F _f )
        {
            IfProviderPtr provider = Helper::makeIfProvider( _f );

            IfSource desc = this->addUnlessProvider( provider );

            return desc;
        }

        template<class F>
        const VectorSources & addSwitch( size_t _count, F _f )
        {
            SwitchProviderPtr provider = Helper::makeSwitchProvider( _f );

            const VectorSources & sources = this->addSwitchProvider( provider, _count );

            return sources;
        }

        template<class F>
        SourcePtr addRepeat( F _f )
        {
            ScopeProviderPtr provider = Helper::makeScopeProvider<F>( _f );

            SourcePtr source = this->addRepeatProvider( provider );

            return source;
        }

        template<class F>
        void addWhile( F _f )
        {
            ScopeProviderPtr provider_scope = Helper::makeScopeProvider<F>( _f );

            this->addWhileProvider( provider_scope );
        }

        template<class FB, class FE>
        SourcePtr addGuard( FB _begin, FE _end )
        {
            GuardProviderPtr begin_provider = Helper::makeGuardProvider( _begin );
            GuardProviderPtr end_provider = Helper::makeGuardProvider( _end );

            SourcePtr source = this->addGuardProvider( begin_provider, end_provider );

            return source;
        }

        void addSemaphore( const SemaphorePtr & _semaphore, uint32_t _flags, int32_t _test, int32_t _apply );

    public:
        void addFunctionProvider( const FunctionProviderPtr & _provider );
        void addCallbackProvider( const CallbackProviderPtr & _provider );
        void addScopeProvider( const ScopeProviderPtr & _provider );
        IfSource addIfProvider( const IfProviderPtr & _provider );
        IfSource addUnlessProvider( const IfProviderPtr & _provider );
        SourcePtr addRepeatProvider( const ScopeProviderPtr & _provider );
        const VectorSources & addSwitchProvider( const SwitchProviderPtr & _provider, size_t _count );
        SourcePtr addGuardProvider( const GuardProviderPtr & _begin, const GuardProviderPtr & _end );
        void addWhileProvider( const ScopeProviderPtr & _providerScope );

    public:
        TaskPtr parse( const ChainPtr & _chain, const TaskPtr & _task );

    protected:
        virtual SourcePtr _provideSource();

    protected:
        VectorTranscriptor m_transcriptors;

        bool m_skip;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<Source> SourcePtr;
    //////////////////////////////////////////////////////////////////////////
}