/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	include "GOAP/TaskIf.h"
#	include "GOAP/Source.h"
#	include "GOAP/IfProvider.h"
#   include "GOAP/Exception.h"

namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
	TaskIf::TaskIf( const IfProviderPtr & _provider, const SourcePtr & _sourceTrue, const SourcePtr & _sourceFalse )
        : Task( TASK_EVENT_RUN )
        , m_provider( _provider )
		, m_sourceTrue( _sourceTrue )
		, m_sourceFalse( _sourceFalse )
	{
	}
	//////////////////////////////////////////////////////////////////////////
	TaskIf::~TaskIf()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	bool TaskIf::_onRun()
	{
		bool result = m_provider->onIf();

		const SourcePtr & result_source = result == true ? m_sourceTrue : m_sourceFalse;

		bool skip = this->isSkip();
		result_source->setSkip( skip );

        if( this->injectSource( result_source ) == false )
        {
            Helper::throw_exception( "TaskIf invalid inject source" );
        }

		return true;
	}
}
