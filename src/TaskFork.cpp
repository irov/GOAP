/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TaskFork.h"

#include "GOAP/NodeInterface.h"
#include "GOAP/SourceInterface.h"
#include "GOAP/SourceProviderInterface.h"

#include "GOAP/Exception.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskFork::TaskFork( const SourceInterfacePtr & _source )
        : m_source( _source )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskFork::~TaskFork()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    const SourceInterfacePtr & TaskFork::getSource() const
    {
        return m_source;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskFork::_onRun( NodeInterface * _node )
    {
        const SourceProviderInterfacePtr & provider = m_source->getSourceProvider();

        bool skip = _node->isSkip();
        provider->setSkip( skip );

        if( _node->forkSource( provider ) == false )
        {
            Helper::throw_exception( "TaskFork invalid fork source" );
        }

        m_source = nullptr;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskFork::_onFinally()
    {
        m_source = nullptr;
    }
}
