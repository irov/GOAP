/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TaskIf.h"

#include "GOAP/SourceInterface.h"
#include "GOAP/SourceProviderInterface.h"
#include "GOAP/NodeInterface.h"
#include "GOAP/IfProviderInterface.h"
#include "GOAP/Exception.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskIf::TaskIf( const IfProviderInterfacePtr & _provider, const SourceInterfacePtr & _sourceTrue, const SourceInterfacePtr & _sourceFalse )
        : m_provider( _provider )
        , m_sourceTrue( _sourceTrue )
        , m_sourceFalse( _sourceFalse )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskIf::~TaskIf()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskIf::_onRun( NodeInterface * _node )
    {
        bool result = m_provider->onIf();

        const SourceInterfacePtr & result_source = (result == true) ? m_sourceTrue : m_sourceFalse;

        bool skip = _node->isSkip();

        const SourceProviderInterfacePtr & provider = result_source->getSourceProvider();

        provider->setSkip( skip );

        if( _node->injectSource( provider ) == false )
        {
            Helper::throw_exception( "TaskIf invalid inject source" );
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskIf::_onFinally()
    {
        m_provider = nullptr;
        m_sourceTrue = nullptr;
        m_sourceFalse = nullptr;
    }
}
