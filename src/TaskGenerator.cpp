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
    TaskGenerator::TaskGenerator( const TimerPtr & _timer, const GeneratorProviderPtr & _provider )
        : m_timer( _timer )
        , m_provider( _provider )
        , m_iterator( 0 )
        , m_time( 0.f )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskGenerator::~TaskGenerator()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskGenerator::_onRun()
    {
        m_timerProvider = m_timer->addTimer( [this]( float _time )
        {
            this->onTime( _time );
        } );

        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskGenerator::_onFastSkip()
    {
        return true;
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
    void TaskGenerator::onTime( float _time )
    {
        m_time += _time;

        for( ;; )
        {
            float delay = m_provider->onDelay( m_iterator );

            if( delay <= 0.f )
            {
                this->complete();

                break;
            }            

            if( m_time < delay )
            {
                break;
            }
             
            SourcePtr source = Helper::makeSource();

            bool skip = this->isSkip();
            source->setSkip( skip );

            m_provider->onEvent( source, m_iterator, delay );

            if( this->forkSource( source ) == false )
            {
                Helper::throw_exception( "TaskFork invalid inject source" );
            }

            m_time -= delay;

            ++m_iterator;
        }
    }
}
