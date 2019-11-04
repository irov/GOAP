/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/SourceBase.h"
#include "GOAP/SourceProvider.h"

#include "GOAP/Node.h"
#include "GOAP/Chain.h"

#include "GOAP/TaskFunction.h"
#include "GOAP/TaskFunctionContext.h"
#include "GOAP/TaskCallback.h"
#include "GOAP/TaskScope.h"
#include "GOAP/TaskSource.h"
#include "GOAP/TaskIf.h"
#include "GOAP/TaskRepeat.h"
#include "GOAP/TaskSwitch.h"
#include "GOAP/TaskFork.h"
#include "GOAP/TaskGuard.h"
#include "GOAP/TaskGenerator.h"
#include "GOAP/TaskBlock.h"
#include "GOAP/TaskNoSkip.h"
#include "GOAP/TaskWhile.h"
#include "GOAP/TaskSemaphore.h"
#include "GOAP/TaskEvent.h"
#include "GOAP/TaskFor.h"
#include "GOAP/TaskTrigger.h"

#include "TranscriptorBase.h"
#include "TranscriptorParallel.h"
#include "TranscriptorRace.h"


namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    SourceBase::SourceBase( const SourceProviderInterfacePtr & _provider )
        : m_provider( _provider )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    SourceBase::~SourceBase()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    const SourceProviderInterfacePtr & SourceBase::getSourceProvider() const
    {
        return m_provider;
    }
    //////////////////////////////////////////////////////////////////////////
    NodePtr SourceBase::makeNode( const TaskInterfacePtr & _task )
    {
        NodePtr task( new Node( _task ) );

        return task;
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addNode( const NodePtr & _task )
    {
        m_provider->addTranscriptor( Helper::makeTranscriptor<TranscriptorBase>( _task ) );
    }
    //////////////////////////////////////////////////////////////////////////
    SourceInterfacePtr SourceBase::makeSource()
    {
        SourceInterfacePtr source = this->_makeSource();

        const SourceProviderInterfacePtr & provider = source->getSourceProvider();

        const SourceProviderInterfacePtr & current = this->getSourceProvider();
        bool skip = current->isSkip();
        provider->setSkip( skip );

        return source;
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addBlock()
    {
        this->addTask<TaskBlock>();
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addNoSkip()
    {
        this->addTask<TaskNoSkip>();
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addSource( const SourceInterfacePtr & _source )
    {
        this->addTask<TaskSource>( _source );
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addEvent( const EventPtr & _event )
    {
        this->addTask<TaskEvent>( _event );
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addSemaphore( const SemaphorePtr & _semaphore, uint32_t _flags, int32_t _test, int32_t _apply )
    {
        this->addTask<TaskSemaphore>( _semaphore, _flags, _test, _apply );
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addSemaphoreEqual( const SemaphorePtr & _semaphore, int32_t _test )
    {
        this->addTask<TaskSemaphore>( _semaphore, FLAG_SEMAPHORE_TEST_EQUAL, _test, 0 );
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addSemaphoreNotEqual( const SemaphorePtr & _semaphore, int32_t _test )
    {
        this->addTask<TaskSemaphore>( _semaphore, FLAG_SEMAPHORE_TEST_NOTEQUAL, _test, 0 );
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addSemaphoreGreater( const SemaphorePtr & _semaphore, int32_t _test )
    {
        this->addTask<TaskSemaphore>( _semaphore, FLAG_SEMAPHORE_TEST_GREATER, _test, 0 );
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addSemaphoreLess( const SemaphorePtr & _semaphore, int32_t _test )
    {
        this->addTask<TaskSemaphore>( _semaphore, FLAG_SEMAPHORE_TEST_LESS, _test, 0 );
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addSemaphoreGreaterEqual( const SemaphorePtr & _semaphore, int32_t _test )
    {   
        this->addTask<TaskSemaphore>( _semaphore, FLAG_SEMAPHORE_TEST_GREATEREQUAL, _test, 0 );
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addSemaphoreLessEqual( const SemaphorePtr & _semaphore, int32_t _test )
    {
        this->addTask<TaskSemaphore>( _semaphore, FLAG_SEMAPHORE_TEST_LESSEQUAL, _test, 0 );
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addSemaphoreAssign( const SemaphorePtr & _semaphore, int32_t _apply )
    {
        this->addTask<TaskSemaphore>( _semaphore, FLAG_SEMAPHORE_APPLY_ASSIGN, 0, _apply );
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addSemaphoreAdd( const SemaphorePtr & _semaphore, int32_t _apply )
    {        
        this->addTask<TaskSemaphore>( _semaphore, FLAG_SEMAPHORE_APPLY_ADD, 0, _apply );
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addSemaphoreSubtract( const SemaphorePtr & _semaphore, int32_t _apply )
    {
        this->addTask<TaskSemaphore>( _semaphore, FLAG_SEMAPHORE_APPLY_SUBTRACT, 0, _apply );
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addWhileProvider( const WhileProviderPtr & _provider )
    {        
        this->addTask<TaskWhile>( _provider );
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addForProvider( const ForProviderPtr & _provider, uint32_t _iterator, uint32_t _count )
    {
        this->addTask<TaskFor>( _provider, _iterator, _count );
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addGeneratorProvider( float _time, uint32_t _iterator, const TimerPtr & _timer, const GeneratorProviderPtr & _provider )
    {
        this->addTask<TaskGenerator>( _time, _iterator, _timer, _provider );
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addTriggerProvider( const EventPtr & _event, const TriggerProviderPtr & _provider )
    {
        this->addTask<TaskTrigger>( _event, _provider );
    }
    //////////////////////////////////////////////////////////////////////////
    ArraySources<2> SourceBase::addIfProvider( const IfProviderPtr & _provider )
    {
        ArraySources<2> sources;

        sources[0] = this->makeSource();
        sources[1] = this->makeSource();

        this->addTaskIf( _provider, sources[0], sources[1] );

        return sources;
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addFunctionProvider( const FunctionProviderPtr & _provider )
    {
        this->addTask<TaskFunction>( _provider );
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addFunctionContextProvider( const FunctionContextProviderPtr & _provider )
    {
        this->addTask<TaskFunctionContext>( _provider );
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addCallbackProvider( const CallbackProviderPtr & _provider )
    {
        this->addTask<TaskCallback>( _provider );
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addScopeProvider( const ScopeProviderPtr & _provider )
    {
        this->addTask<TaskScope>( _provider );
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addTaskFork( const SourceInterfacePtr & _source )
    {
        this->addTask<TaskFork>( _source );
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addTaskGuard( const GuardProviderPtr & _begin, const GuardProviderPtr & _end )
    {
        this->addTask<TaskGuard>( _begin, _end );
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addTaskIf( const IfProviderPtr & _provider, const SourceInterfacePtr & _sourceTrue, const SourceInterfacePtr & _sourceFalse )
    {
        this->addTask<TaskIf>( _provider, _sourceTrue, _sourceFalse );
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addTaskRepeat( const WhileProviderPtr & _provider, const SourceInterfacePtr & _sourceUntil )
    {
        this->addTask<TaskRepeat>( _provider, _sourceUntil );
    }
    //////////////////////////////////////////////////////////////////////////
    const VectorSources & SourceBase::addParallelTranscriptor( uint32_t _count )
    {
        VectorSources sources;
        this->makeSources_( sources, _count );

        TranscriptorParallelPtr transcriptor = Helper::makeTranscriptor<TranscriptorParallel>( std::move( sources ) );

        m_provider->addTranscriptor( transcriptor );

        const VectorSources & sources_parallel = transcriptor->getSources();

        return sources_parallel;
    }
    //////////////////////////////////////////////////////////////////////////
    const VectorSources & SourceBase::addRaceTranscriptor( uint32_t _count )
    {
        VectorSources sources;
        this->makeSources_( sources, _count );

        TranscriptorRacePtr transcriptor = Helper::makeTranscriptor<TranscriptorRace>( std::move( sources ) );

        m_provider->addTranscriptor( transcriptor );

        const VectorSources & sources_race = transcriptor->getSources();

        return sources_race;
    }
    //////////////////////////////////////////////////////////////////////////
    const VectorSources & SourceBase::addTaskSwitch( uint32_t _count, const SwitchProviderPtr & _provider )
    {
        VectorSources sources;
        this->makeSources_( sources, _count );

        TaskSwitchPtr task = this->emplaceTask<TaskSwitch>( _provider, std::move( sources ) );

        const VectorSources & sources_switch = task->getSources();

        return sources_switch;
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::makeSources_( VectorSources & _sources, uint32_t _count )
    {
        _sources.resize( _count );

        for( SourceInterfacePtr & source : _sources )
        {
            source = this->makeSource();
        }
    }
    //////////////////////////////////////////////////////////////////////////
}