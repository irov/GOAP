/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TaskFor.h"

#include "GOAP/Cook.h"
#include "GOAP/ForProviderInterface.h"

#include "GOAP/Exception.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskFor::TaskFor( Allocator * _allocator, const ForProviderInterfacePtr & _providerFor, uint32_t _iterator, uint32_t _count )
        : TaskInterface( _allocator )
        , m_providerFor( _providerFor )
        , m_iterator( _iterator )
        , m_count( _count )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskFor::~TaskFor()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskFor::_onCheck()
    {
        if( m_iterator == m_count )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskFor::_onRun( NodeInterface * _node )
    {
        SourceInterfacePtr source = _node->makeSource();

        ForProviderInterfacePtr providerFor = std::move( m_providerFor );

        if( providerFor->onFor( source, m_iterator, m_count ) == false )
        {
            return true;
        }

        Cook::addForProvider( source, providerFor, m_iterator + 1, m_count );

        const SourceProviderInterfacePtr & provider = source->getSourceProvider();

        if( _node->injectSource( provider ) == false )
        {
            Helper::throw_exception( "TaskFor invalid inject source" );
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskFor::_onFinally()
    {
        m_providerFor = nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
}
