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
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class ForProvider> ForProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    class TaskFor
        : public TaskInterface
    {
        GOAP_DECLARE_VISITABLE( TaskInterface );

    public:
        TaskFor( const ForProviderPtr & _providerFor, uint32_t _iterator, uint32_t _count );
        ~TaskFor() override;

    public:
        bool _onCheck() override;
        bool _onRun( NodeInterface * _node ) override;
        void _onFinally() override;

    protected:
        ForProviderPtr m_providerFor;
        uint32_t m_iterator;
        uint32_t m_count;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskFor> TaskForPtr;
    //////////////////////////////////////////////////////////////////////////
}