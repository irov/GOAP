/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "TaskCallback.h"

#include "GOAP/NodeInterface.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    TaskCallback::TaskCallback( const CallbackProviderInterfacePtr & _provider )
        : m_provider( _provider )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskCallback::~TaskCallback()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskCallback::_onRun( NodeInterface * _node )
    {
        bool skip = _node->isSkip();

        Allocator * allocator = this->getAllocator();

        CallbackObserverInterfacePtr callback = Helper::makeCallbackObserver( allocator, [_node]( bool _skip )
        {
            _node->complete( true, _skip );
        } );

        m_provider->onCallbackProvider( callback, skip );

        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskCallback::_onFinally()
    {
        m_provider = nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskCallback::_onSkipable() const
    {
        return false;
    }
}