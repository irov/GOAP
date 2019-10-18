/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/Alias.h"

#include "GOAP/Source.h"

#include "GOAP/Exception.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    Alias::Alias()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Alias::~Alias()
    {
    }
    //////////////////////////////////////////////////////////////////////////    
    bool Alias::_onRun( NodeInterface * _task )
    {
        SourcePtr source = Helper::makeSource();

        SourcePtr guard_source = source->addGuard( [this]()
        {
            this->incref();
        }
            , [this]()
        {
            this->_onAliasFinally();

            this->decref();
        } );

        this->_onAliasGenerate( guard_source );

        const SourceProviderInterfacePtr & provider = source->getSourceProvider();

        if( _task->injectSource( provider ) == false )
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