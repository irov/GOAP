/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/Source.h"

#include "GOAP/Task.h"
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
#include "GOAP/TaskWhile.h"
#include "GOAP/TaskSemaphore.h"
#include "GOAP/TaskEvent.h"
#include "GOAP/TaskFor.h"

#include "TranscriptorBase.h"
#include "TranscriptorParallel.h"
#include "TranscriptorRace.h"


namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    Source::Source()
        : m_skip( false )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Source::~Source()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::setSkip( bool _skip )
    {
        m_skip = _skip;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Source::isSkip() const
    {
        return m_skip;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Source::empty() const
    {
        bool result = m_transcriptors.empty();

        return result;
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addTask( const TaskPtr & _task )
    {        
        m_transcriptors.emplace_back( new TranscriptorBase( _task ) );
    }
    //////////////////////////////////////////////////////////////////////////
    ArraySources<2> Source::tryTask( const TaskPtr & _task )
    {
        this->addTask( _task );

        ArraySources<2> desc = this->addIf( [_task]()
        {
            if( _task->isError() == true )
            {
                return false;
            }

            return true;
        } );

        return desc;
    }
    //////////////////////////////////////////////////////////////////////////
    const VectorSources & Source::addParallel( uint32_t _count )
    {
        VectorSources sources;
        this->makeSources_( sources, _count );

        TranscriptorParallelPtr transcriptor = m_transcriptors.emplace_back( new TranscriptorParallel( std::move( sources ) ) );

        const VectorSources & transcriptor_sources = transcriptor->getSources();

        return transcriptor_sources;
    }
    //////////////////////////////////////////////////////////////////////////
    const VectorSources & Source::addRace( uint32_t _count )
    {
        VectorSources sources;
        this->makeSources_( sources, _count );

        TranscriptorRacePtr transcriptor = m_transcriptors.emplace_back( new TranscriptorRace( std::move( sources ) ) );

        const VectorSources & transcriptor_sources = transcriptor->getSources();

        return transcriptor_sources;
    }
    //////////////////////////////////////////////////////////////////////////
    SourcePtr Source::addFork()
    {
        SourcePtr source = this->_provideSource();

        this->addTask<TaskFork>( source );

        return source;
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addBlock()
    {
        this->addTask<TaskBlock>();
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addSource( const SourcePtr & _source )
    {
        this->addTask<TaskSource>( _source );
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addEvent( const EventPtr & _event )
    {
        this->addTask<TaskEvent>( _event );
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addSemaphore( const SemaphorePtr & _semaphore, uint32_t _flags, int32_t _test, int32_t _apply )
    {
        this->addTask<TaskSemaphore>( _semaphore, _flags, _test, _apply );
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addSemaphoreEqual( const SemaphorePtr & _semaphore, int32_t _test )
    {
        this->addTask<TaskSemaphore>( _semaphore, FLAG_SEMAPHORE_TEST_EQUAL, _test, 0 );
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addSemaphoreNotEqual( const SemaphorePtr & _semaphore, int32_t _test )
    {
        this->addTask<TaskSemaphore>( _semaphore, FLAG_SEMAPHORE_TEST_NOTEQUAL, _test, 0 );
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addSemaphoreGreater( const SemaphorePtr & _semaphore, int32_t _test )
    {
        this->addTask<TaskSemaphore>( _semaphore, FLAG_SEMAPHORE_TEST_GREATER, _test, 0 );
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addSemaphoreLess( const SemaphorePtr & _semaphore, int32_t _test )
    {
        this->addTask<TaskSemaphore>( _semaphore, FLAG_SEMAPHORE_TEST_LESS, _test, 0 );
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addSemaphoreGreaterEqual( const SemaphorePtr & _semaphore, int32_t _test )
    {   
        this->addTask<TaskSemaphore>( _semaphore, FLAG_SEMAPHORE_TEST_GREATEREQUAL, _test, 0 );
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addSemaphoreLessEqual( const SemaphorePtr & _semaphore, int32_t _test )
    {
        this->addTask<TaskSemaphore>( _semaphore, FLAG_SEMAPHORE_TEST_LESSEQUAL, _test, 0 );
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addSemaphoreAssign( const SemaphorePtr & _semaphore, int32_t _apply )
    {
        this->addTask<TaskSemaphore>( _semaphore, FLAG_SEMAPHORE_APPLY_ASSIGN, 0, _apply );
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addSemaphoreAdd( const SemaphorePtr & _semaphore, int32_t _apply )
    {        
        this->addTask<TaskSemaphore>( _semaphore, FLAG_SEMAPHORE_APPLY_ADD, 0, _apply );
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addSemaphoreSubtract( const SemaphorePtr & _semaphore, int32_t _apply )
    {
        this->addTask<TaskSemaphore>( _semaphore, FLAG_SEMAPHORE_APPLY_SUBTRACT, 0, _apply );
    }
    //////////////////////////////////////////////////////////////////////////
    SourcePtr Source::addRepeatProvider( const WhileProviderPtr & _provider )
    {
        SourcePtr source_until = this->_provideSource();

        this->addTask<TaskRepeat>( _provider, source_until );

        return source_until;
    }
    //////////////////////////////////////////////////////////////////////////
    const VectorSources & Source::addSwitchProvider( const SwitchProviderPtr & _provider, uint32_t _count )
    {
        VectorSources sources;
        this->makeSources_( sources, _count );

        TaskSwitchPtr task = this->emplaceTask<TaskSwitch>( _provider, std::move( sources ) );

        const VectorSources & sources_switch = task->getSources();

        return sources_switch;
    }
    //////////////////////////////////////////////////////////////////////////
    SourcePtr Source::addGuardProvider( const GuardProviderPtr & _begin, const GuardProviderPtr & _end )
    {
        const VectorSources & race_source = this->addRace( 2 );

        const SourcePtr & source_guard = race_source[0];

        source_guard->addTask<TaskGuard>( _begin, _end );

        const SourcePtr & source_code = race_source[1];

        return source_code;
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addWhileProvider( const WhileProviderPtr & _provider )
    {        
        this->addTask<TaskWhile>( _provider );
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addForProvider( const ForProviderPtr & _provider, uint32_t _iterator, uint32_t _count )
    {
        this->addTask<TaskFor>( _provider, _iterator, _count );
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addGeneratorProvider( const TimerPtr & _timer, const GeneratorProviderPtr & _provider )
    {
        this->addTask<TaskGenerator>( _timer, _provider );
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addFunctionProvider( const FunctionProviderPtr & _provider )
    {
        this->addTask<TaskFunction>( _provider );
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addFunctionContextProvider( const FunctionContextProviderPtr & _provider )
    {
        this->addTask<TaskFunctionContext>( _provider );
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addCallbackProvider( const CallbackProviderPtr & _provider )
    {
        this->addTask<TaskCallback>( _provider );
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addScopeProvider( const ScopeProviderPtr & _provider )
    {
        this->addTask<TaskScope>( _provider );
    }
    //////////////////////////////////////////////////////////////////////////
    ArraySources<2> Source::addIfProvider( const IfProviderPtr & _provider )
    {
        SourcePtr source_true = this->_provideSource();
        SourcePtr source_false = this->_provideSource();

        this->addTask<TaskIf>( _provider, source_true, source_false );

        return ArraySources<2>{source_true, source_false};
    }
    //////////////////////////////////////////////////////////////////////////
    ArraySources<2> Source::addUnlessProvider( const IfProviderPtr & _provider )
    {
        SourcePtr source_true = this->_provideSource();
        SourcePtr source_false = this->_provideSource();
                
        this->addTask<TaskIf>( _provider, source_true, source_false );

        return ArraySources<2>{source_false, source_true};
    }
    //////////////////////////////////////////////////////////////////////////
    TaskPtr Source::parse( const ChainPtr & _chain, const TaskPtr & _task )
    {
        TaskPtr current_task = _task;

        for( const TranscriptorPtr & description : m_transcriptors )
        {
            TaskPtr last_task = description->generate( _chain, current_task );

            current_task = last_task;
        }

        m_transcriptors.clear();

        return current_task;
    }
    //////////////////////////////////////////////////////////////////////////
    SourcePtr Source::_provideSource()
    {
        return Helper::makeSource();
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::makeSources_( VectorSources & _sources, uint32_t _count )
    {
        _sources.resize( _count );

        for( SourcePtr & source : _sources )
        {
            source = this->_provideSource();
        }
    }
}