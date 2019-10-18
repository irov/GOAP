/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/TaskRepeat.h"
#include "GOAP/Source.h"

#include "GOAP/Exception.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskRepeat::TaskRepeat( const WhileProviderPtr & _providerRepeat, const SourcePtr & _until )
        : m_providerRepeat( _providerRepeat )
        , m_sourceUntil( _until )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskRepeat::~TaskRepeat()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskRepeat::_onRun( NodeInterface * _task )
    {
        SourcePtr source = _task->makeSource();

        auto [source_while, source_until] = source->addRace<2>();

        source_while->addWhileProvider( m_providerRepeat );
        source_until->addSource( m_sourceUntil );

        const SourceProviderInterfacePtr & provider = source->getSourceProvider();

        if( _task->injectSource( provider ) == false )
        {
            Helper::throw_exception( "TaskRepeat invalid inject source" );
        }

        m_providerRepeat = nullptr;
        m_sourceUntil = nullptr;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskRepeat::_onFinalize()
    {
        m_providerRepeat = nullptr;
        m_sourceUntil = nullptr;
    }
}
