/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/TaskCallback.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class TaskCallback::TaskCallbackObserver
        : public CallbackObserver
    {
    public:
        explicit TaskCallbackObserver( TaskCallback * _task )
            : m_task( _task )
        {
        }

    protected:
        void onCallback( bool _skip ) override
        {
            m_task->complete( true, _skip );
        }

    protected:
        TaskCallback * m_task;
    };
    //////////////////////////////////////////////////////////////////////////
    TaskCallback::TaskCallback( const CallbackProviderPtr & _provider )
        : m_provider( _provider )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskCallback::~TaskCallback()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskCallback::_onRun()
    {
        bool skip = this->isSkip();

		CallbackObserverPtr callback = GOAP_NEW TaskCallbackObserver( this );

		m_provider->onCallback( callback, skip );

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