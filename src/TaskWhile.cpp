/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
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
    TaskWhile::TaskWhile( const ScopeProviderPtr & _providerWhile )
        : m_providerWhile( _providerWhile )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskWhile::~TaskWhile()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskWhile::_onRun()
    {
        SourcePtr source = Helper::makeSource();

        bool skip = this->isSkip();
        source->setSkip( skip );

        if( m_providerWhile->onScope( source ) == false )
        {
            return true;
        }

        source->addWhileProvider( m_providerWhile );

        m_providerWhile = nullptr;

        this->injectSource( source );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskWhile::_onFinalize()
    {
        m_providerWhile = nullptr;
    }
}
