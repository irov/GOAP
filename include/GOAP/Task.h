/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/TaskInterface.h"

namespace GOAP
{
    class Task
        : public TaskInterface
    {
    protected:
        Task();
        ~Task() override;

    protected:
        bool _onInitialize() override;
        void _onFinalize() override;

    protected:
        bool _onValidate() const override;
        bool _onCheck() override;
        bool _onRun( NodeInterface * _node ) override;
        bool _onSkipable() const override;
        void _onSkipNoSkiped() override;
        bool _onSkipBlock() override;
        void _onComplete() override;
        bool _onFastSkip() override;
        void _onSkip() override;
        void _onCancel() override;
        void _onFinally() override;
        bool _onCheckRun( const NodeInterface * _node ) const override;
        bool _onCheckSkip( const NodeInterface * _node ) const override;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<Task> TaskPtr;
    //////////////////////////////////////////////////////////////////////////
}