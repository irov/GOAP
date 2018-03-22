/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/TaskFor.h"
#include "GOAP/Source.h"
#include "GOAP/Chain.h"
#include "GOAP/ForProvider.h"

#include "GOAP/Exception.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class TaskFor::ChainProviderEnd
        : public ChainProvider
    {
    public:
        ChainProviderEnd( TaskFor * _task )
            : m_task( _task )
        {
        }

    public:
        void onChain( bool _skip ) override
        {
            if( m_task->iterateComplete_( _skip ) == true )
            {
                m_task->complete( true, _skip );
            }
        }

    protected:
        TaskFor * m_task;
    };
    //////////////////////////////////////////////////////////////////////////
    TaskFor::TaskFor( const ForProviderPtr & _provider, uint32_t _count )
        : m_provider( _provider )
        , m_count( _count )
        , m_iterator( 0 )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskFor::~TaskFor()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskFor::_onCheck()
    {
        if( m_count == 0 )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskFor::_onRun()
    {
        bool skip = this->isSkip();

        if( this->iterateComplete_( skip ) == true )
        {
            return true;
        }

        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskFor::_onSkip()
    {
        if( m_chainIterator != nullptr )
        {
            m_chainIterator->skip();
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskFor::_onFinally()
    {
        if( m_chainIterator != nullptr )
        {
            m_chainIterator->cancel();
            m_chainIterator = nullptr;
        }

        m_provider = nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskFor::iterateComplete_( bool _skip )
    {
        if( m_provider == nullptr )
        {
            return true;
        }

        uint32_t iterator = m_iterator++;

        if( iterator == m_count )
        {
            return true;
        }

        GOAP::SourcePtr source = GOAP_NEW GOAP::Source();

        source->setSkip( _skip );

        if( m_provider->onFor( source, iterator, m_count ) == false )
        {
            return true;
        }

        ChainPtr chainIterator = GOAP_NEW Chain( source );

        chainIterator->setCallbackProvider( GOAP_NEW ChainProviderEnd( this ) );

        m_chainIterator = chainIterator;

        if( m_chainIterator->run() == false )
        {
            return true;
        }

        return false;
    }
}
