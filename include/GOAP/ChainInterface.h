/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Factorable.h"
#include "GOAP/IntrusivePtr.h"
#include "GOAP/ChainProviderInterface.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class KernelInterface;
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class NodeInterface> NodeInterfacePtr;
    typedef IntrusivePtr<class SourceInterface> SourceInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    class ChainInterface
        : public Factorable
    {
    public:
        ChainInterface( Allocator * _allocator )
            : Factorable( _allocator )
        {
        }

    public:
        virtual void setCallbackProvider( const ChainProviderInterfacePtr & _cb ) = 0;
        virtual const ChainProviderInterfacePtr & getCallbackProvider() const = 0;

    public:
        virtual const SourceInterfacePtr & getSource() const = 0;

    public:
        virtual bool run() = 0;
        virtual void skip() = 0;
        virtual void cancel() = 0;

    public:
        virtual void runNode( const NodeInterfacePtr & _task ) = 0;
        virtual void completeNode( const NodeInterfacePtr & _task ) = 0;
        virtual void processNode( const NodeInterfacePtr & _task, bool _skip ) = 0;

    public:
        virtual bool isComplete() const = 0;
        virtual bool isCancel() const = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<ChainInterface> ChainInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
}