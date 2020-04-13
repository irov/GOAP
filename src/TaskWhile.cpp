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

        bool injecting = m_provider->onWhile( source );

        if( injecting == true )
        {
            Cook::addWhileProvider( source, m_provider );

            const SourceProviderInterfacePtr & provider = source->getSourceProvider();

            if( _node->injectSource( provider ) == false )
            {
                Helper::throw_exception( "TaskWhile invalid inject source" );
            }
        }

        m_provider = nullptr;

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
