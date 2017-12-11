/*
* Copyright (C) 2017, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	pragma once

#	include "GOAP/Task.h"

#	include "GOAP/CallbackProvider.h"

namespace GOAP
{
    class TaskCallback
        : public Task
        , public CallbackObserver
    {
        DECLARE_VISITABLE( Task );

    public:
        explicit TaskCallback( const CallbackProviderPtr & _provider );
        ~TaskCallback() override;

    public:
        bool _onRun() override;
        void _onFinally() override;
        bool _onSkipable() const override;

    protected:
        void onCallback( bool _skip ) override;

    protected:
        CallbackProviderPtr m_provider;
    };
}