/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	include "GOAP/Source.h"

#	include "GOAP/Task.h"
#	include "GOAP/Chain.h"
#   include "GOAP/Config.h"

#	include "GOAP/FunctionProvider.h"
#	include "GOAP/TaskFunction.h"
#	include "GOAP/TaskCallback.h"
#	include "GOAP/TaskScope.h"
#	include "GOAP/TaskIf.h"
#	include "GOAP/TaskRepeat.h"
#	include "GOAP/TaskSwitch.h"
#	include "GOAP/TaskFork.h"
#	include "GOAP/TaskGuard.h"
#	include "GOAP/TaskDeadLock.h"
#   include "GOAP/TaskWhile.h"

#	include "TranscriptorBase.h"
#	include "TranscriptorParallel.h"
#	include "TranscriptorRace.h"


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
    void Source::addTask( const TaskPtr & _task )
    {
        TranscriptorBase * description = GOAP_NEW TranscriptorBase( _task );

        m_transcriptors.push_back( description );
    }
    //////////////////////////////////////////////////////////////////////////
    const VectorSources & Source::addParallel( size_t _count )
    {
        TranscriptorParallel * description = GOAP_NEW TranscriptorParallel( _count );

        m_transcriptors.push_back( description );

        const VectorSources & sources = description->getSources();

        return sources;
    }
    //////////////////////////////////////////////////////////////////////////
    const VectorSources & Source::addRace( size_t _count )
    {
        TranscriptorRace * description = GOAP_NEW TranscriptorRace( _count );

        m_transcriptors.push_back( description );

        const VectorSources & sources = description->getSources();

        return sources;
    }
    //////////////////////////////////////////////////////////////////////////
    SourcePtr Source::addFork()
    {
        SourcePtr source = this->_provideSource();

        TaskPtr task_fork = GOAP_NEW TaskFork( source );

        this->addTask( task_fork );

        return source;
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addDeadLock()
    {
        this->addTask( GOAP_NEW TaskDeadLock() );
    }
    //////////////////////////////////////////////////////////////////////////
    SourcePtr Source::addRepeatProvider( const ScopeProviderPtr & _provider )
    {
        SourcePtr source_until = this->_provideSource();

        TaskPtr task = GOAP_NEW TaskRepeat( _provider, source_until );

        this->addTask( task );

        return source_until;
    }
    //////////////////////////////////////////////////////////////////////////
    const VectorSources & Source::addSwitchProvider( const SwitchProviderPtr & _provider, size_t _count )
    {
        VectorSources sources;
        sources.resize( _count );

        for( VectorSources::iterator
            it = sources.begin(),
            it_end = sources.end();
            it != it_end;
            ++it )
        {
            SourcePtr & source = *it;

            source = this->_provideSource();
        }

        TaskSwitchPtr task = GOAP_NEW TaskSwitch( _provider, sources );

        this->addTask( task );

        const VectorSources & sources_switch = task->getSources();

        return sources_switch;
    }
    //////////////////////////////////////////////////////////////////////////
    SourcePtr Source::addGuardProvider( const GuardProviderPtr & _begin, const GuardProviderPtr & _end )
    {
        const VectorSources & race_source = this->addRace( 2 );

        const SourcePtr & source_guard = race_source[0];

        TaskGuardPtr task = GOAP_NEW TaskGuard( _begin, _end );

        source_guard->addTask( task );

        const SourcePtr & source_code = race_source[1];

        return source_code;
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addWhileProvider( const ScopeProviderPtr & _scopeProvider )
    {
        TaskPtr task = GOAP_NEW TaskWhile( _scopeProvider );

        this->addTask( task );
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addFunctionProvider( const FunctionProviderPtr & _provider )
    {
        TaskPtr task = GOAP_NEW TaskFunction( _provider );

        this->addTask( task );
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addCallbackProvider( const CallbackProviderPtr & _provider )
    {
        TaskPtr task = GOAP_NEW TaskCallback( _provider );

        this->addTask( task );
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addScopeProvider( const ScopeProviderPtr & _provider )
    {
        TaskPtr task = GOAP_NEW TaskScope( _provider );

        this->addTask( task );
    }
    //////////////////////////////////////////////////////////////////////////
    IfSource Source::addIfProvider( const IfProviderPtr & _provider )
    {
        SourcePtr source_true = this->_provideSource();
        SourcePtr source_false = this->_provideSource();

        TaskPtr task = GOAP_NEW TaskIf( _provider, source_true, source_false );

        this->addTask( task );

        IfSource desc;
        desc.source_true = source_true;
        desc.source_false = source_false;

        return desc;
    }
    //////////////////////////////////////////////////////////////////////////
    IfSource Source::addUnlessProvider( const IfProviderPtr & _provider )
    {
        SourcePtr source_true = this->_provideSource();
        SourcePtr source_false = this->_provideSource();

        TaskPtr task = GOAP_NEW TaskIf( _provider, source_true, source_false );

        this->addTask( task );

        IfSource desc;
        desc.source_true = source_false;
        desc.source_false = source_true;

        return desc;
    }
    //////////////////////////////////////////////////////////////////////////
    TaskPtr Source::parse( const ChainPtr & _chain, const TaskPtr & _task )
    {
        TaskPtr current_task = _task;

        for( VectorTranscriptor::iterator
            it = m_transcriptors.begin(),
            it_end = m_transcriptors.end();
            it != it_end;
            ++it )
        {
            const TranscriptorPtr & description = *it;

            TaskPtr last_task = description->generate( _chain, current_task );

            current_task = last_task;
        }

        return current_task;
    }
    //////////////////////////////////////////////////////////////////////////
    SourcePtr Source::_provideSource()
    {
        return GOAP_NEW Source();
    }
}