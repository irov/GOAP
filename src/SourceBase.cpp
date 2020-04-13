/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "SourceBase.h"
#include "SourceProvider.h"

#include "GOAP/NodeInterface.h"

#include "TranscriptorBase.h"
#include "TranscriptorParallel.h"
#include "TranscriptorRace.h"


namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    SourceBase::SourceBase( const KernelInterfacePtr & _kernel, const SourceProviderInterfacePtr & _provider )
        : m_kernel( _kernel )
        , m_provider( _provider )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    SourceBase::~SourceBase()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    const SourceProviderInterfacePtr & SourceBase::getSourceProvider() const
    {
        return m_provider;
    }
    //////////////////////////////////////////////////////////////////////////
    NodeInterfacePtr SourceBase::makeNode( const TaskInterfacePtr & _task )
    {
        NodeInterfacePtr task = m_kernel->makeNode( _task );

        return task;
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::addNode( const NodeInterfacePtr & _task )
    {
        Allocator * allocator = m_kernel->getAllocator();

        TranscriptorBasePtr transcriptor = Helper::makeTranscriptor<TranscriptorBase>( allocator, _task );

        m_provider->addTranscriptor( transcriptor );
    }
    //////////////////////////////////////////////////////////////////////////
    SourceInterfacePtr SourceBase::makeSource()
    {
        SourceInterfacePtr source = this->_makeSource();

        const SourceProviderInterfacePtr & provider = source->getSourceProvider();

        const SourceProviderInterfacePtr & current = this->getSourceProvider();
        bool skip = current->isSkip();
        provider->setSkip( skip );

        return source;
    }
    //////////////////////////////////////////////////////////////////////////
    Allocator * SourceBase::getAllocator() const
    {
        Allocator * allocator = m_kernel->getAllocator();

        return allocator;
    }
    //////////////////////////////////////////////////////////////////////////
    const VectorSources & SourceBase::addParallelTranscriptor( uint32_t _count )
    {
        VectorSources sources;
        this->makeSources_( sources, _count );

        Allocator * allocator = m_kernel->getAllocator();

        TranscriptorParallelPtr transcriptor = Helper::makeTranscriptor<TranscriptorParallel>( allocator, std::move( sources ) );

        m_provider->addTranscriptor( transcriptor );

        const VectorSources & sources_parallel = transcriptor->getSources();

        return sources_parallel;
    }
    //////////////////////////////////////////////////////////////////////////
    const VectorSources & SourceBase::addRaceTranscriptor( uint32_t _count )
    {
        VectorSources sources;
        this->makeSources_( sources, _count );

        Allocator * allocator = m_kernel->getAllocator();

        TranscriptorRacePtr transcriptor = Helper::makeTranscriptor<TranscriptorRace>( allocator, std::move( sources ) );

        m_provider->addTranscriptor( transcriptor );

        const VectorSources & sources_race = transcriptor->getSources();

        return sources_race;
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceBase::makeSources_( VectorSources & _sources, uint32_t _count )
    {
        _sources.resize( _count );

        for( SourceInterfacePtr & source : _sources )
        {
            source = this->makeSource();
        }
    }
    //////////////////////////////////////////////////////////////////////////
}