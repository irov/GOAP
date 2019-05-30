/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/TaskWhile.h"
#include "GOAP/Source.h"
#include "GOAP/Chain.h"
#include "GOAP/ChainProvider.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class TaskWhile::ChainProviderWhileEnd
        : public ChainProvider
    {
    public:
        explicit ChainProviderWhileEnd( TaskWhile * _task )
            : m_task( _task )
        {
        }

    public:
        void onChain( bool _skip ) override
        {
            m_task->whileComplete_( _skip );
        }

    protected:
        TaskWhile * m_task;
    };
    //////////////////////////////////////////////////////////////////////////
    TaskWhile::TaskWhile( const ScopeProviderPtr & _providerScope )
        : m_providerScope( _providerScope )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskWhile::~TaskWhile()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskWhile::_onFinalize()
    {
        m_providerScope = nullptr;

        if( m_chainWhile != nullptr )
        {
            ChainPtr chain = m_chainWhile;
            m_chainWhile = nullptr;
            chain->cancel();
        }
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskWhile::_onRun()
    {
        SourcePtr sourceWhile = GOAP_NEW Source();

        bool skip = this->isSkip();
        sourceWhile->setSkip( skip );

        if( m_providerScope->onScope( sourceWhile ) == false )
        {
            return true;
        }

        ChainPtr chainWhile = GOAP_NEW Chain( sourceWhile );

		ChainProviderPtr chainProviderWhileEnd = GOAP_NEW ChainProviderWhileEnd( this );

        chainWhile->setCallbackProvider( chainProviderWhileEnd );

        m_chainWhile = chainWhile;

        if( m_chainWhile->run() == false )
        {
            return true;
        }

        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskWhile::_onSkip()
    {
        m_providerScope = nullptr;

        if( m_chainWhile != nullptr )
        {
            m_chainWhile->skip();
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskWhile::whileComplete_( bool _skip )
    {
        if( m_providerScope == nullptr )
        {
            return;
        }

        SourcePtr sourceWhile = GOAP_NEW Source();

        sourceWhile->setSkip( _skip );

        if( m_providerScope->onScope( sourceWhile ) == false )
        {
            this->complete( true, _skip );

            return;
        }

        ChainPtr chainWhile = GOAP_NEW Chain( sourceWhile );
        
		ChainProviderPtr chainProviderWhileEnd = GOAP_NEW ChainProviderWhileEnd( this );

        chainWhile->setCallbackProvider( chainProviderWhileEnd );

        m_chainWhile = chainWhile;

        if( m_chainWhile->run() == false )
        {
            this->complete( true, _skip );

            return;
        }
    }
}
