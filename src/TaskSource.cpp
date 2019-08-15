/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/TaskSource.h"
#include "GOAP/Source.h"

#include "GOAP/Exception.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskSource::TaskSource( const SourcePtr & _source )
        : m_source( _source )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskSource::~TaskSource()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskSource::_onRun()
    {
        if( m_source != nullptr )
        {
            if( this->injectSource( m_source ) == false )
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
