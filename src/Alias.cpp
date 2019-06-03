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
	//////////////////////////////////////////////////////////////////////////
    class Alias::BeginGuardProvider
        : public GuardProvider
    {
    public:
        explicit BeginGuardProvider( Alias * _alias )
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
        explicit EndGuardProvider( Alias * _alias )
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
        SourcePtr source = new Source();

        bool skiped = this->isSkip();
        source->setSkip( skiped );

		GuardProviderPtr begin = new BeginGuardProvider( this );
		GuardProviderPtr end = new EndGuardProvider( this );

        SourcePtr guard_source = source->addGuardProvider( begin, end );

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