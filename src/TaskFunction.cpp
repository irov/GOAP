/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	include "GOAP/TaskFunction.h"

#	include "GOAP/FunctionProvider.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskFunction::TaskFunction( const FunctionProviderPtr & _provider )
        : Task( TASK_EVENT_RUN )
        , m_provider( _provider )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskFunction::~TaskFunction()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskFunction::_onRun()
    {
        if( m_provider != nullptr )
        {
            m_provider->onFunction();
        }

        return true;
    }
}