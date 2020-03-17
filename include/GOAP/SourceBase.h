/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/KernelInterface.h"
#include "GOAP/SourceInterface.h"
#include "GOAP/SourceProviderInterface.h"
#include "GOAP/NodeInterface.h"

#include "GOAP/FunctionProvider.h"
#include "GOAP/FunctionContextProvider.h"
#include "GOAP/CallbackProvider.h"
#include "GOAP/ScopeProvider.h"
#include "GOAP/IfProvider.h"
#include "GOAP/IfProviderMember.h"
#include "GOAP/TriggerProvider.h"
#include "GOAP/SwitchProvider.h"
#include "GOAP/GuardProvider.h"
#include "GOAP/ForProvider.h"
#include "GOAP/GeneratorProvider.h"
#include "GOAP/SemaphoreFlags.h"
#include "GOAP/WhileProvider.h"
#include "GOAP/TranscriptorParallelArray.h"
#include "GOAP/TranscriptorRaceArray.h"

#include "GOAP/Vector.h"
#include "GOAP/Zip.h"
#include "GOAP/Timer.h"

namespace GOAP
{
    class SourceBase
        : public SourceInterface
    {
    public:
        SourceBase( const KernelInterfacePtr & _kernel, const SourceProviderInterfacePtr & _provider );
        ~SourceBase() override;

    public:
        const SourceProviderInterfacePtr & getSourceProvider() const override;

    public:
        SourceInterfacePtr makeSource() override;

    protected:
        virtual SourceInterfacePtr _makeSource() = 0;

    protected:
        Allocator * getAllocator() const override;

    public:
        NodeInterfacePtr makeNode( const TaskInterfacePtr & _provider ) override;
        void addNode( const NodeInterfacePtr & _task ) override;

    public:
        const VectorSources & addParallelTranscriptor( uint32_t _count ) override;
        const VectorSources & addRaceTranscriptor( uint32_t _count ) override;

    protected:
        void makeSources_( VectorSources & _sources, uint32_t _count );

    protected:
        KernelInterfacePtr m_kernel;
        SourceProviderInterfacePtr m_provider;
    };
}