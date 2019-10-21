/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/TaskGenerator.h"
#include "GOAP/GeneratorProvider.h"

#include "GOAP/Source.h"

#include "GOAP/Exception.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskGenerator::TaskGenerator( float _time, uint32_t _iterator, const TimerPtr & _timer, const GeneratorProviderPtr & _provider )
        : m_time( _time )
        , m_iterator( _iterator )
        , m_timer( _timer )
        , m_provider( _provider )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskGenerator::~TaskGenerator()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskGenerator::_onRun( NodeInterface * _task )
    {
        m_timerProvider = m_timer->addTimer( [this, _task]( float _time )
        {
            this->onTime( _task, _time );
        } );

        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskGenerator::_onFastSkip()
    {
        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskGenerator::_onCancel()
    {
        m_provider = nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskGenerator::_onFinalize()
    {
        m_provider = nullptr;

        m_timer->removeTimerProvider( m_timerProvider );
        m_timerProvider = nullptr;
        m_timer = nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskGenerator::onTime( NodeInterface * _task, float _time )
    {
        m_time += _time;

        float delay = m_provider->onDelay( m_iterator );

        if( delay <= 0.f )
        {
            _task->complete();

            return;
        }

        if( m_time < delay )
        {
            return;
        }

        m_time -= delay;

        uint32_t new_iterator = m_iterator + 1;

        SourcePtr source = _task->makeSource();

        auto && [source_generator, source_fork] = source->addParallel<2>();

        source_generator->addGeneratorProvider( m_time, new_iterator, m_timer, m_provider );

        SourceInterfacePtr source_event = source_fork->addFork();

        m_provider->onEvent( source_event, m_iterator, delay );

        const SourceProviderInterfacePtr & provider = source->getSourceProvider();

        _task->injectSource( provider );

        _task->complete();
    }
}
