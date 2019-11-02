/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/TaskRepeat.h"
#include "GOAP/Source.h"
#include "GOAP/NodeInterface.h"

#include "GOAP/Exception.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskRepeat::TaskRepeat( const WhileProviderPtr & _provider, const SourceInterfacePtr & _until )
        : m_provider( _provider )
        , m_sourceUntil( _until )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskRepeat::~TaskRepeat()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskRepeat::_onRun( NodeInterface * _node )
    {
        SourcePtr source = _node->makeSource();

        auto [source_while, source_until] = source->addRace<2>();

        source_while->addWhileProvider( m_provider );
        source_until->addSource( m_sourceUntil );

        const SourceProviderInterfacePtr & provider = source->getSourceProvider();

        if( _node->injectSource( provider ) == false )
        {
            Helper::throw_exception( "TaskRepeat invalid inject source" );
        }

        m_provider = nullptr;
        m_sourceUntil = nullptr;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskRepeat::_onFinalize()
    {
        m_provider = nullptr;
        m_sourceUntil = nullptr;
    }
}
