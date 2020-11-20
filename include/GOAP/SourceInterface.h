/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Factorable.h"
#include "GOAP/Allocator.h"
#include "GOAP/IntrusivePtr.h"
#include "GOAP/ArraySources.h"
#include "GOAP/VectorSources.h"
#include "GOAP/ViewSources.h"
#include "GOAP/Zip.h"

#include "GOAP/SourceProviderInterface.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class KernelInterface;
    //////////////////////////////////////////////////////////////////////////    
    typedef IntrusivePtr<class SourceInterface> SourceInterfacePtr;
    typedef IntrusivePtr<class NodeInterface> NodeInterfacePtr;
    typedef IntrusivePtr<class TaskInterface> TaskInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    class SourceInterface
        : public Factorable
    {
    public:
        virtual KernelInterface * getKernel() const = 0;

    public:
        virtual Allocator * getAllocator() const = 0;

    public:
        virtual const SourceProviderInterfacePtr & getSourceProvider() const = 0;

    public:
        virtual SourceInterfacePtr makeSource() = 0;

    public:
        virtual void addNode( const NodeInterfacePtr & _task ) = 0;

    public:
        virtual const VectorSources & addRaceTranscriptor( uint32_t _count ) = 0;
        virtual const VectorSources & addParallelTranscriptor( uint32_t _count ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<SourceInterface> SourceInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
}