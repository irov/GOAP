/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Factorable.h"
#include "GOAP/IntrusivePtr.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class SourceProviderInterface> SourceProviderInterfacePtr;
    typedef IntrusivePtr<class SourceInterface> SourceInterfacePtr;
    typedef IntrusivePtr<class NodeInterface> NodeInterfacePtr;
    typedef IntrusivePtr<class TaskInterface> TaskInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    class SourceInterface
        : public Factorable
    {
    public:
        virtual const SourceProviderInterfacePtr & getSourceProvider() const = 0;

    public:
        virtual SourceInterfacePtr makeSource() = 0;

    public:
        virtual NodeInterfacePtr makeNode( const TaskInterfacePtr & _provider ) = 0;
        virtual void addNode( const NodeInterfacePtr & _task ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<SourceInterface> SourceInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
}