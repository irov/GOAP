/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TaskGuard.h"

#include "GOAP/GuardProviderInterface.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskGuard::TaskGuard( Allocator * _allocator, const GuardProviderInterfacePtr & _begin, const GuardProviderInterfacePtr & _end )
        : TaskInterface( _allocator )
        , m_begin( _begin )
        , m_end( _end )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskGuard::~TaskGuard()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskGuard::_onRun( NodeInterface * _node )
    {
        GOAP_UNUSED( _node );

        m_begin->onGuard();

        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskGuard::_onCancel()
    {
        m_end->onGuard();
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskGuard::_onFinally()
    {
        m_begin = nullptr;
        m_end = nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskGuard::_onSkipable() const
    {
        return false;
    }
}
