/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TaskWhile.h"

#include "GOAP/Cook.h"
#include "GOAP/Exception.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskWhile::TaskWhile( const WhileProviderInterfacePtr & _provider )
        : m_provider( _provider )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskWhile::~TaskWhile()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskWhile::_onRun( NodeInterface * _node )
    {
        SourceInterfacePtr source = _node->makeSource();

        WhileProviderInterfacePtr provider = std::move( m_provider );

        bool injecting = provider->onWhile( source );

        if( injecting == true )
        {
            Cook::addWhileProvider( source, provider );

            const SourceProviderInterfacePtr & source_provider = source->getSourceProvider();

            if( _node->injectSource( source_provider ) == false )
            {
                Helper::throw_exception( "TaskWhile invalid inject source" );
            }
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskWhile::_onFastSkip()
    {
        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskWhile::_onFinalize()
    {
        m_provider = nullptr;
    }
}
