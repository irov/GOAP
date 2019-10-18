/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/TaskIf.h"
#include "GOAP/Source.h"
#include "GOAP/IfProvider.h"
#include "GOAP/Exception.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskIf::TaskIf( const IfProviderPtr & _provider, const SourcePtr & _sourceTrue, const SourcePtr & _sourceFalse )
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
    bool TaskIf::_onRun( NodeInterface * _task )
    {
        bool result = m_provider->onIf();

        const SourcePtr & result_source = (result == true) ? m_sourceTrue : m_sourceFalse;

        bool skip = _task->isSkip();

        const SourceProviderInterfacePtr & provider = result_source->getSourceProvider();

        provider->setSkip( skip );

        if( _task->injectSource( provider ) == false )
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
