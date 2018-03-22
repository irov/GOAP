/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/TaskRepeat.h"
#include "GOAP/Source.h"
#include "GOAP/Chain.h"
#include "GOAP/ChainProvider.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class TaskRepeat::ChainProviderUntilEnd
        : public ChainProvider
    {
    public:
        explicit ChainProviderUntilEnd( TaskRepeat * _task )
            : m_task( _task )
        {
        }

    public:
        void onChain( bool _skip ) override
        {
            m_task->untilComplete_( _skip );
        }

    protected:
        TaskRepeat * m_task;
    };
    //////////////////////////////////////////////////////////////////////////
    class TaskRepeat::ChainProviderRepeatEnd
        : public ChainProvider
    {
    public:
        explicit ChainProviderRepeatEnd( TaskRepeat * _task )
            : m_task( _task )
        {
        }

    public:
        void onChain( bool _skip ) override
        {
            m_task->repeatComplete_( _skip );
        }

    protected:
        TaskRepeat * m_task;
    };
    //////////////////////////////////////////////////////////////////////////
    TaskRepeat::TaskRepeat( const ScopeProviderPtr & _provider, const SourcePtr & _until )
        : m_providerRepeat( _provider )
        , m_sourceUntil( _until )
        , m_repeat( true )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskRepeat::~TaskRepeat()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskRepeat::_onFinalize()
    {
        m_repeat = false;

        m_providerRepeat = nullptr;
        m_sourceUntil = nullptr;

        if( m_chainRepeat != nullptr )
        {
            ChainPtr chain = m_chainRepeat;
            m_chainRepeat = nullptr;
            chain->cancel();
        }

        if( m_chainUntil != nullptr )
        {
            ChainPtr chain = m_chainUntil;
            m_chainUntil = nullptr;
            chain->cancel();
        }
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskRepeat::_onRun()
    {
        ChainPtr chainUntil = GOAP_NEW Chain( m_sourceUntil );

        chainUntil->setCallbackProvider( GOAP_NEW ChainProviderUntilEnd( this ) );

        m_chainUntil = chainUntil;

        if( m_chainUntil->run() == false )
        {
            return true;
        }

        if( m_repeat == false )
        {
            return true;
        }

        GOAP::SourcePtr sourceRepeat = GOAP_NEW GOAP::Source();

        bool skip = this->isSkip();
        sourceRepeat->setSkip( skip );

        if( m_providerRepeat->onScope( sourceRepeat ) == false )
        {
            return true;
        }

        ChainPtr chainRepeat = GOAP_NEW Chain( sourceRepeat );

        chainRepeat->setCallbackProvider( GOAP_NEW ChainProviderRepeatEnd( this ) );

        m_chainRepeat = chainRepeat;

        if( m_chainRepeat->run() == false )
        {
            return true;
        }

        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskRepeat::repeatComplete_( bool _skip )
    {
        if( m_repeat == false )
        {
            return;
        }

        GOAP::SourcePtr sourceRepeat = GOAP_NEW GOAP::Source();

        sourceRepeat->setSkip( _skip );

        if( m_providerRepeat->onScope( sourceRepeat ) == false )
        {
            this->complete( true, _skip );

            return;
        }

        ChainPtr chainRepeat = GOAP_NEW Chain( sourceRepeat );
        
        chainRepeat->setCallbackProvider( GOAP_NEW ChainProviderRepeatEnd( this ) );

        m_chainRepeat = chainRepeat;

        if( m_chainRepeat->run() == false )
        {
            this->complete( true, _skip );

            return;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskRepeat::untilComplete_( bool _skip )
    {
        m_repeat = false;

        if( m_chainRepeat != nullptr )
        {
            ChainPtr chain = m_chainRepeat;
            m_chainRepeat = nullptr;
            chain->cancel();
        }

        this->complete( true, _skip );
    }
}
