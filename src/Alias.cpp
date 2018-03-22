/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/Alias.h"

#include "GOAP/Source.h"
#include "GOAP/Chain.h"

#include "GOAP/Exception.h"

namespace GOAP
{
    class Alias::BeginGuardProvider
        : public GuardProvider
    {
    public:
        BeginGuardProvider( Alias * _alias )
            : m_alias( _alias )
        {
        }

    public:
        void onGuard() override
        {
            IntrusiveThisAcquire( m_alias );
        }

    protected:
        Alias * m_alias;
    };
    //////////////////////////////////////////////////////////////////////////
    class Alias::EndGuardProvider
        : public GuardProvider
    {
    public:
        EndGuardProvider( Alias * _alias )
            : m_alias( _alias )
        {
        }

    public:
        void onGuard() override
        {
            m_alias->_onAliasFinally();

            IntrusiveThisRelease( m_alias );
        }

    protected:
        Alias * m_alias;
    };
    //////////////////////////////////////////////////////////////////////////
    Alias::Alias()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Alias::~Alias()
    {
    }
    //////////////////////////////////////////////////////////////////////////    
    bool Alias::_onRun()
    {
        GOAP::SourcePtr source = GOAP_NEW GOAP::Source();

        bool skiped = this->isSkip();
        source->setSkip( skiped );

        SourcePtr guard_source = source->addGuardProvider( GOAP_NEW BeginGuardProvider( this ), GOAP_NEW EndGuardProvider( this ) );

        this->_onAliasGenerate( guard_source );

        if( this->injectSource( source ) == false )
        {
            Helper::throw_exception( "Alias invalid inject source" );
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void Alias::_onFinally()
    {
        //Empty
    }
    //////////////////////////////////////////////////////////////////////////
    void Alias::_onAliasFinally()
    {
        //Empty
    }
}