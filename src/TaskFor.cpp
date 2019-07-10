/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/TaskFor.h"
#include "GOAP/Source.h"
#include "GOAP/Chain.h"
#include "GOAP/ForProvider.h"

#include "GOAP/Exception.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskFor::TaskFor( const ForProviderPtr & _providerFor, uint32_t _iterator, uint32_t _count )
        : m_providerFor( _providerFor )
        , m_iterator( _iterator )
        , m_count( _count )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskFor::~TaskFor()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskFor::_onCheck()
    {
        if( m_iterator == m_count )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskFor::_onRun()
    {
        SourcePtr source = new Source();

        bool skip = this->isSkip();
        source->setSkip( skip );

        if( m_providerFor->onFor( source, m_iterator, m_count ) == false )
        {
            return true;
        }

        source->addForProvider( m_providerFor, m_iterator + 1, m_count );

        m_providerFor = nullptr;

        this->injectSource( source );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskFor::_onFinally()
    {
        m_providerFor = nullptr;
    }
}
