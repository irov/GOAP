/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TaskSource.h"

#include "GOAP/SourceInterface.h"

#include "GOAP/Exception.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskSource::TaskSource( Allocator * _allocator, const SourceInterfacePtr & _source )
        : TaskInterface( _allocator )
        , m_source( _source )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskSource::~TaskSource()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskSource::_onRun( NodeInterface * _node )
    {
        if( m_source != nullptr )
        {
            const SourceProviderInterfacePtr & provider = m_source->getSourceProvider();

            if( _node->injectSource( provider ) == false )
            {
                Helper::throw_exception( "TaskSource invalid inject source" );
            }

            m_source = nullptr;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskSource::_onFinally()
    {
        m_source = nullptr;
    }
}
