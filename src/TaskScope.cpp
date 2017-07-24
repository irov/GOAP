/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	include "GOAP/TaskScope.h"
#	include "GOAP/Source.h"
#	include "GOAP/ScopeProvider.h"

#   include "GOAP/Exception.h"

namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
	TaskScope::TaskScope( const ScopeProviderPtr & _provider )
        : Task( TASK_EVENT_RUN )
		, m_provider(_provider)
	{
	}
	//////////////////////////////////////////////////////////////////////////
	TaskScope::~TaskScope()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	bool TaskScope::_onRun()
	{
		GOAP::SourcePtr source = new GOAP::Source();

		bool skip = this->isSkip();
		source->setSkip( skip );

		if( m_provider->onScope( source ) == false )
		{
			return true;
		}

        if( this->injectSource( source ) == false )
        {
            Helper::throw_exception( "TaskScope invalid inject source" );
        }

		return true;
	}
}
