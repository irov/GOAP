/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/Cook.h"

#include "SourceProvider.h"

#include "Node.h"

#include "TaskFunction.h"
#include "TaskFunctionContext.h"
#include "TaskCallback.h"
#include "TaskScope.h"
#include "TaskSource.h"
#include "TaskIf.h"
#include "TaskRepeat.h"
#include "TaskSwitch.h"
#include "TaskFork.h"
#include "TaskGuard.h"
#include "TaskGenerator.h"
#include "TaskBlock.h"
#include "TaskNoSkip.h"
#include "TaskWhile.h"
#include "TaskSemaphore.h"
#include "TaskEvent.h"
#include "TaskFor.h"
#include "TaskTrigger.h"

#include "IfProviderMember.h"

#include "TranscriptorBase.h"
#include "TranscriptorParallel.h"
#include "TranscriptorRace.h"


namespace GOAP
{
    namespace Cook
    {
        //////////////////////////////////////////////////////////////////////////
        void addBlock( const SourceInterfacePtr & _source )
        {
            addTask<TaskBlock>( _source );
        }
        //////////////////////////////////////////////////////////////////////////
        void addNoSkip( const SourceInterfacePtr & _source )
        {
            addTask<TaskNoSkip>( _source );
        }
        //////////////////////////////////////////////////////////////////////////
        ViewSources<SourceInterface> addParallel( const SourceInterfacePtr & _source, uint32_t _count )
        {
            const VectorSources & transcriptor_sources = _source->addParallelTranscriptor( _count );

            return ViewSources<SourceInterface>( transcriptor_sources );
        }
        //////////////////////////////////////////////////////////////////////////
        ViewSources<SourceInterface> addRace( const SourceInterfacePtr & _source, uint32_t _count )
        {
            const VectorSources & transcriptor_sources = _source->addRaceTranscriptor( _count );

            return ViewSources<SourceInterface>( transcriptor_sources );
        }
        //////////////////////////////////////////////////////////////////////////
        SourceInterfacePtr addFork( const SourceInterfacePtr & _source )
        {
            SourceInterfacePtr source = _source->makeSource();

            addTaskFork( _source, source );

            return source;
        }
        //////////////////////////////////////////////////////////////////////////
        ArrayTypeSources<SourceInterface, 2> addIf( const SourceInterfacePtr & _source, bool * _member )
        {
            Allocator * allocator = _source->getAllocator();

            IfProviderInterfacePtr provider = Helper::makeIfProviderMember( allocator, _member );

            ArraySources<2> sources = addIfProvider( _source, provider );

            return Helper::ArraySourcesCast<SourceInterface>( sources );
        }
        //////////////////////////////////////////////////////////////////////////
        void addSource( const SourceInterfacePtr & _source, const SourceInterfacePtr & _other )
        {
            addTask<TaskSource>( _source, _other );
        }
        //////////////////////////////////////////////////////////////////////////
        void addEvent( const SourceInterfacePtr & _source, const EventInterfacePtr & _event )
        {
            addTask<TaskEvent>( _source, _event );
        }
        //////////////////////////////////////////////////////////////////////////
        void addSemaphore( const SourceInterfacePtr & _source, const SemaphoreInterfacePtr & _semaphore, uint32_t _flags, int32_t _test, int32_t _apply )
        {
            addTask<TaskSemaphore>( _source, _semaphore, _flags, _test, _apply );
        }
        //////////////////////////////////////////////////////////////////////////
        void addSemaphoreEqual( const SourceInterfacePtr & _source, const SemaphoreInterfacePtr & _semaphore, int32_t _test )
        {
            addTask<TaskSemaphore>( _source, _semaphore, FLAG_SEMAPHORE_TEST_EQUAL, _test, 0 );
        }
        //////////////////////////////////////////////////////////////////////////
        void addSemaphoreNotEqual( const SourceInterfacePtr & _source, const SemaphoreInterfacePtr & _semaphore, int32_t _test )
        {
            addTask<TaskSemaphore>( _source, _semaphore, FLAG_SEMAPHORE_TEST_NOTEQUAL, _test, 0 );
        }
        //////////////////////////////////////////////////////////////////////////
        void addSemaphoreGreater( const SourceInterfacePtr & _source, const SemaphoreInterfacePtr & _semaphore, int32_t _test )
        {
            addTask<TaskSemaphore>( _source, _semaphore, FLAG_SEMAPHORE_TEST_GREATER, _test, 0 );
        }
        //////////////////////////////////////////////////////////////////////////
        void addSemaphoreLess( const SourceInterfacePtr & _source, const SemaphoreInterfacePtr & _semaphore, int32_t _test )
        {
            addTask<TaskSemaphore>( _source, _semaphore, FLAG_SEMAPHORE_TEST_LESS, _test, 0 );
        }
        //////////////////////////////////////////////////////////////////////////
        void addSemaphoreGreaterEqual( const SourceInterfacePtr & _source, const SemaphoreInterfacePtr & _semaphore, int32_t _test )
        {
            addTask<TaskSemaphore>( _source, _semaphore, FLAG_SEMAPHORE_TEST_GREATEREQUAL, _test, 0 );
        }
        //////////////////////////////////////////////////////////////////////////
        void addSemaphoreLessEqual( const SourceInterfacePtr & _source, const SemaphoreInterfacePtr & _semaphore, int32_t _test )
        {
            addTask<TaskSemaphore>( _source, _semaphore, FLAG_SEMAPHORE_TEST_LESSEQUAL, _test, 0 );
        }
        //////////////////////////////////////////////////////////////////////////
        void addSemaphoreAssign( const SourceInterfacePtr & _source, const SemaphoreInterfacePtr & _semaphore, int32_t _apply )
        {
            addTask<TaskSemaphore>( _source, _semaphore, FLAG_SEMAPHORE_APPLY_ASSIGN, 0, _apply );
        }
        //////////////////////////////////////////////////////////////////////////
        void addSemaphoreAdd( const SourceInterfacePtr & _source, const SemaphoreInterfacePtr & _semaphore, int32_t _apply )
        {
            addTask<TaskSemaphore>( _source, _semaphore, FLAG_SEMAPHORE_APPLY_ADD, 0, _apply );
        }
        //////////////////////////////////////////////////////////////////////////
        void addSemaphoreSubtract( const SourceInterfacePtr & _source, const SemaphoreInterfacePtr & _semaphore, int32_t _apply )
        {
            addTask<TaskSemaphore>( _source, _semaphore, FLAG_SEMAPHORE_APPLY_SUBTRACT, 0, _apply );
        }
        //////////////////////////////////////////////////////////////////////////
        void addWhileProvider( const SourceInterfacePtr & _source, const WhileProviderInterfacePtr & _provider )
        {
            addTask<TaskWhile>( _source, _provider );
        }
        //////////////////////////////////////////////////////////////////////////
        void addForProvider( const SourceInterfacePtr & _source, const ForProviderInterfacePtr & _provider, uint32_t _iterator, uint32_t _count )
        {
            addTask<TaskFor>( _source, _provider, _iterator, _count );
        }
        //////////////////////////////////////////////////////////////////////////
        void addGeneratorProvider( const SourceInterfacePtr & _source, float _time, uint32_t _iterator, const TimerInterfacePtr & _timer, const GeneratorProviderInterfacePtr & _provider )
        {
            addTask<TaskGenerator>( _source, _time, _iterator, _timer, _provider );
        }
        //////////////////////////////////////////////////////////////////////////
        void addTriggerProvider( const SourceInterfacePtr & _source, const EventInterfacePtr & _event, const TriggerProviderInterfacePtr & _provider )
        {
            addTask<TaskTrigger>( _source, _event, _provider );
        }
        //////////////////////////////////////////////////////////////////////////
        ArraySources<2> addIfProvider( const SourceInterfacePtr & _source, const IfProviderInterfacePtr & _provider )
        {
            ArraySources<2> sources;

            sources[0] = _source->makeSource();
            sources[1] = _source->makeSource();

            addTaskIf( _source, _provider, sources[0], sources[1] );

            return sources;
        }
        //////////////////////////////////////////////////////////////////////////
        void addFunctionProvider( const SourceInterfacePtr & _source, const FunctionProviderInterfacePtr & _provider )
        {
            addTask<TaskFunction>( _source, _provider );
        }
        //////////////////////////////////////////////////////////////////////////
        void addFunctionContextProvider( const SourceInterfacePtr & _source, const FunctionContextProviderInterfacePtr & _provider )
        {
            addTask<TaskFunctionContext>( _source, _provider );
        }
        //////////////////////////////////////////////////////////////////////////
        void addCallbackProvider( const SourceInterfacePtr & _source, const CallbackProviderInterfacePtr & _provider )
        {
            addTask<TaskCallback>( _source, _provider );
        }
        //////////////////////////////////////////////////////////////////////////
        void addScopeProvider( const SourceInterfacePtr & _source, const ScopeProviderInterfacePtr & _provider )
        {
            addTask<TaskScope>( _source, _provider );
        }
        //////////////////////////////////////////////////////////////////////////
        void addTaskFork( const SourceInterfacePtr & _source, const SourceInterfacePtr & _other )
        {
            addTask<TaskFork>( _source, _other );
        }
        //////////////////////////////////////////////////////////////////////////
        void addTaskGuard( const SourceInterfacePtr & _source, const GuardProviderInterfacePtr & _begin, const GuardProviderInterfacePtr & _end )
        {
            addTask<TaskGuard>( _source, _begin, _end );
        }
        //////////////////////////////////////////////////////////////////////////
        void addTaskIf( const SourceInterfacePtr & _source, const IfProviderInterfacePtr & _provider, const SourceInterfacePtr & _sourceTrue, const SourceInterfacePtr & _sourceFalse )
        {
            addTask<TaskIf>( _source, _provider, _sourceTrue, _sourceFalse );
        }
        //////////////////////////////////////////////////////////////////////////
        void addTaskRepeat( const SourceInterfacePtr & _source, const WhileProviderInterfacePtr & _provider, const SourceInterfacePtr & _sourceUntil )
        {
            addTask<TaskRepeat>( _source, _provider, _sourceUntil );
        }
        //////////////////////////////////////////////////////////////////////////
        namespace Detail
        {
            static void makeSources( const SourceInterfacePtr & _source, VectorSources & _sources, uint32_t _count )
            {
                _sources.resize( _count );

                for( SourceInterfacePtr & source : _sources )
                {
                    source = _source->makeSource();
                }
            }
        }
        //////////////////////////////////////////////////////////////////////////
        const VectorSources & addParallelTranscriptor( const SourceInterfacePtr & _source, uint32_t _count )
        {
            VectorSources sources;
            Detail::makeSources( _source, sources, _count );

            Allocator * allocator = _source->getAllocator();

            TranscriptorParallelPtr transcriptor = Helper::makeTranscriptor<TranscriptorParallel>( allocator, std::move( sources ) );

            const SourceProviderInterfacePtr & provider = _source->getSourceProvider();

            provider->addTranscriptor( transcriptor );

            const VectorSources & sources_parallel = transcriptor->getSources();

            return sources_parallel;
        }
        //////////////////////////////////////////////////////////////////////////
        const VectorSources & addRaceTranscriptor( const SourceInterfacePtr & _source, uint32_t _count )
        {
            VectorSources sources;
            Detail::makeSources( _source, sources, _count );

            Allocator * allocator = _source->getAllocator();

            TranscriptorRacePtr transcriptor = Helper::makeTranscriptor<TranscriptorRace>( allocator, std::move( sources ) );

            const SourceProviderInterfacePtr & provider = _source->getSourceProvider();

            provider->addTranscriptor( transcriptor );

            const VectorSources & sources_race = transcriptor->getSources();

            return sources_race;
        }
        //////////////////////////////////////////////////////////////////////////
        const VectorSources & addTaskSwitch( const SourceInterfacePtr & _source, uint32_t _count, const SwitchProviderInterfacePtr & _provider )
        {
            VectorSources sources;
            Detail::makeSources( _source, sources, _count );

            TaskSwitchPtr task = emplaceTask<TaskSwitch>( _source, _provider, std::move( sources ) );

            const VectorSources & sources_switch = task->getSources();

            return sources_switch;
        }
        //////////////////////////////////////////////////////////////////////////
    }
}