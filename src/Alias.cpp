/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	include "GOAP/Alias.h"

#   include "GOAP/Source.h"
#   include "GOAP/Chain.h"

#   include "GOAP/Exception.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    Alias::Alias()        
    {
    }
    //////////////////////////////////////////////////////////////////////////    
    bool Alias::_onRun()
    {
        GOAP::SourcePtr source = GOAP_NEW GOAP::Source();

        bool skiped = this->isSkip();
        source->setSkip( skiped );

        SourcePtr guard_source = source->addGuard( [this](){ IntrusiveThisAcquire( this ); }, [this](){ this->_onAliasFinally(); IntrusiveThisRelease( this ); } );

        this->_onGenerate( guard_source );

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