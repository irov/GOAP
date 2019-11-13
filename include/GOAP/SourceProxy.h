/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/SourceBase.h"

#include "GOAP/TaskGuard.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    template<class Type>
    class SourceProxy
        : public SourceBase
    {
    public:
        using SourceBase::SourceBase;

    public:
        typedef IntrusivePtr<Type> TypeSourcePtr;

    public:
        template<class T, class ... Args>
        ArraySources<2> tryTask( Args && ... _args )
        {
            TaskInterfacePtr provider = Helper::makeTask<T>( std::forward<Args &&>( _args ) ... );

            NodeInterfacePtr node = this->makeNode( provider );

            this->addNode( node );

            ArraySources<2> desc = this->addIf( [node]()
            {
                if( node->isError() == true )
                {
                    return false;
                }

                return true;
            } );

            return desc;
        }

    public:
        ViewSources<Type> addParallel( uint32_t _count )
        {
            const VectorSources & transcriptor_sources = this->addParallelTranscriptor( _count );

            return ViewSources<Type>( transcriptor_sources );
        }

        template<size_t Count>
        ArrayTypeSources<Type, Count> addParallel()
        {
            ArraySources<Count> sources;

            for( SourceInterfacePtr & source : sources )
            {
                source = this->makeSource();
            }

            TranscriptorParallelArrayPtr<Count> transcriptor = Helper::makeTranscriptorParallelArray( std::move( sources ) );

            m_provider->addTranscriptor( transcriptor );

            const ArraySources<Count> & transcriptor_sources = transcriptor->getSources();

            return Helper::ArraySourcesCast<Type>( transcriptor_sources );
        }

        ViewSources<Type> addRace( uint32_t _count )
        {
            const VectorSources & transcriptor_sources = this->addRaceTranscriptor( _count );

            return ViewSources<Type>( transcriptor_sources );
        }

        template<size_t Count>
        ArrayTypeSources<Type, Count> addRace()
        {
            ArraySources<Count> sources;

            for( SourceInterfacePtr & source : sources )
            {
                source = this->makeSource();
            }

            TranscriptorRaceArrayPtr<Count> transcriptor = Helper::makeTranscriptorRaceArray( std::move( sources ) );

            m_provider->addTranscriptor( transcriptor );

            const ArraySources<Count> & transcriptor_sources = transcriptor->getSources();

            return Helper::ArraySourcesCast<Type>( transcriptor_sources );
        }

        TypeSourcePtr addFork()
        {
            TypeSourcePtr source = this->makeSource();

            this->addTaskFork( source );

            return source;
        }

        template<class F>
        void addTrigger( const EventPtr & _event, F _f )
        {
            TriggerProviderPtr provider = Helper::makeTriggerProvider( _f );

            this->addTriggerProvider( _event, provider );
        }

        ArrayTypeSources<Type, 2> addIf( bool * _member )
        {
            IfProviderPtr provider = Helper::makeIfProviderMember( _member );

            ArraySources<2> sources = this->addIfProvider( provider );

            return Helper::ArraySourcesCast<Type>( sources );
        }

        template<class F>
        ArrayTypeSources<Type, 2> addIf( F _f )
        {
            IfProviderPtr provider = Helper::makeIfProvider( _f );

            ArraySources<2> sources = this->addIfProvider( provider );

            return Helper::ArraySourcesCast<Type>( sources );
        }

        template<class C, class M, class ... Args>
        ArrayTypeSources<Type, 2> addIf( C * _self, M _method, Args && ... _args )
        {
            IfProviderPtr provider = Helper::makeIfProvider( [&, _self, _method, _args ...](){ return (_self->*_method)(_args ...); } );

            ArraySources<2> sources = this->addIfProvider( provider );

            return Helper::ArraySourcesCast<Type>( sources );
        }

        template<class F>
        TypeSourcePtr addIfSuccessful( F _f )
        {
            IfProviderPtr provider = Helper::makeIfProvider( _f );

            ArraySources<2> sources = this->addIfProvider( provider );

            return sources[0];
        }

        template<class C, class M, class ... Args>
        TypeSourcePtr addIfSuccessful( C * _self, M _method, Args && ... _args )
        {
            IfProviderPtr provider = Helper::makeIfProvider( [&, _self, _method, _args ...](){ return (_self->*_method)(_args ...); } );

            ArraySources<2> sources = this->addIfProvider( provider );

            return sources[0];
        }

        template<class F>
        TypeSourcePtr addIfFailure( F _f )
        {
            IfProviderPtr provider = Helper::makeIfProvider( _f );

            ArraySources<2> sources = this->addIfProvider( provider );

            return sources[1];
        }

        template<class C, class M, class ... Args>
        TypeSourcePtr addIfFailure( C * _self, M _method, Args && ... _args )
        {
            IfProviderPtr provider = Helper::makeIfProvider( [&, _self, _method, _args ...](){ return (_self->*_method)(_args ...); } );

            ArraySources<2> sources = this->addIfProvider( provider );

            return sources[1];
        }

        template<class F>
        ArrayTypeSources<Type, 2> addUnless( F _f )
        {
            IfProviderPtr provider = Helper::makeIfProvider( _f );

            ArraySources<2> sources = this->addIfProvider( provider );

            return ArrayTypeSources<Type, 2>{sources[1], sources[0]};
        }

        template<class C, class M, class ... Args>
        ArrayTypeSources<Type, 2> addUnless( C * _self, M _method, Args && ... _args )
        {
            IfProviderPtr provider = Helper::makeIfProvider( [&, _self, _method, _args ...](){ return (_self->*_method)(_args ...); } );

            ArraySources<2> sources = this->addIfProvider( provider );

            return ArrayTypeSources<Type, 2>{sources[1], sources[0]};
        }

        template<class F>
        ViewSources<Type> addSwitch( uint32_t _count, F _f )
        {
            SwitchProviderPtr provider = Helper::makeSwitchProvider( _f );

            const VectorSources & sources_switch = this->addTaskSwitch( _count, provider );

            return ViewSources<Type>( sources_switch );
        }

        template<class F>
        TypeSourcePtr addRepeat( F _f )
        {
            WhileProviderPtr provider = Helper::makeWhileProvider( _f );

            TypeSourcePtr source_until = this->makeSource();

            this->addTaskRepeat( provider, source_until );

            return source_until;
        }

        template<class C, class M, class ... Args>
        TypeSourcePtr addRepeat( C * _self, M _method, Args && ... _args )
        {
            WhileProviderPtr provider = Helper::makeWhileProvider( [&, _self, _method, _args ...]( const SourceInterfacePtr & _source ){ return (_self->*_method)((TypeSourcePtr)_source, _args ...); } );

            TypeSourcePtr source_until = this->makeSource();

            this->addTaskRepeat( provider, source_until );

            return source_until;
        }

        template<class FB, class FE>
        TypeSourcePtr addGuard( FB _begin, FE _end )
        {
            GuardProviderPtr begin_provider = Helper::makeGuardProvider( _begin );
            GuardProviderPtr end_provider = Helper::makeGuardProvider( _end );

            auto && [source_guard, source_code] = this->addRace<2>();

            source_guard->addTaskGuard( begin_provider, end_provider );

            return TypeSourcePtr( source_code );
        }

    public:
        template<class C>
        Zip<TypeSourcePtr, typename ViewSources<Type>::iterator, typename C::iterator> addParallelZip( C & _c )
        {
            typename C::size_type zip_size = _c.size();
            ViewSources<Type> parallel_sources = this->addParallel( (uint32_t)zip_size );

            return Helper::makeZip<TypeSourcePtr>( parallel_sources.begin(), parallel_sources.end(), _c.begin(), _c.end() );
        }

        template<class C>
        Zip<TypeSourcePtr, typename ViewSources<Type>::iterator, typename C::const_iterator> addParallelZip( const C & _c )
        {
            typename C::size_type zip_size = _c.size();
            ViewSources<Type> parallel_sources = this->addParallel( (uint32_t)zip_size );

            return Helper::makeZip<TypeSourcePtr>( parallel_sources.begin(), parallel_sources.end(), _c.begin(), _c.end() );
        }

        template<class C>
        Zip<TypeSourcePtr, typename ViewSources<Type>::iterator, typename C::iterator> addRaceZip( C & _c )
        {
            typename C::size_type zip_size = _c.size();
            ViewSources<Type> parallel_sources = this->addRace( (uint32_t)zip_size );

            return Helper::makeZip<TypeSourcePtr>( parallel_sources.begin(), parallel_sources.end(), _c.begin(), _c.end() );
        }

        template<class C>
        Zip<TypeSourcePtr, typename ViewSources<Type>::iterator, typename C::const_iterator> addRaceZip( const C & _c )
        {
            typename C::size_type zip_size = _c.size();
            ViewSources<Type> parallel_sources = this->addRace( (uint32_t)zip_size );

            return Helper::makeZip<TypeSourcePtr>( parallel_sources.begin(), parallel_sources.end(), _c.begin(), _c.end() );
        }

        template<class C, class F>
        Zip<TypeSourcePtr, typename ViewSources<Type>::iterator, typename C::iterator> addSwitchZip( C & _c, F _f )
        {
            typename C::size_type zip_size = _c.size();
            ViewSources<Type> parallel_sources = this->addSwitch( (uint32_t)zip_size, _f );

            return Helper::makeZip<TypeSourcePtr>( parallel_sources.begin(), parallel_sources.end(), _c.begin(), _c.end() );
        }

        template<class C, class F>
        Zip<TypeSourcePtr, typename ViewSources<Type>::iterator, typename C::const_iterator> addSwitchZip( const C & _c, F _f )
        {
            typename C::size_type zip_size = _c.size();
            ViewSources<Type> parallel_sources = this->addSwitch( (uint32_t)zip_size, _f );

            return Helper::makeZip<TypeSourcePtr>( parallel_sources.begin(), parallel_sources.end(), _c.begin(), _c.end() );
        }
    };
    //////////////////////////////////////////////////////////////////////////
}