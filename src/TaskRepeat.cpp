/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TaskRepeat.h"

#include "GOAP/Cook.h"
#include "GOAP/NodeInterface.h"
#include "GOAP/Exception.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskRepeat::TaskRepeat( const WhileProviderInterfacePtr & _provider, const SourceInterfacePtr & _until )
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
        SourceInterfacePtr source = _node->makeSource();

        auto [source_while, source_until] = Cook::addRace<2>( source );

        Cook::addWhileProvider( source_while, std::move( m_provider ) );
        Cook::addSource( source_until, std::move( m_sourceUntil ) );

        const SourceProviderInterfacePtr & provider = source->getSourceProvider();

        if( _node->injectSource( provider ) == false )
        {
            Helper::throw_exception( "TaskRepeat invalid inject source" );
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskRepeat::_onFinalize()
    {
        m_provider = nullptr;
        m_sourceUntil = nullptr;
    }
}
