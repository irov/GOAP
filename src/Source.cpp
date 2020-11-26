/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "Source.h"
#include "SourceProvider.h"

#include "GOAP/NodeInterface.h"

#include "TranscriptorBase.h"
#include "TranscriptorParallel.h"
#include "TranscriptorRace.h"


namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    Source::Source( Allocator * _allocator, const SourceProviderInterfacePtr & _provider )
        : SourceInterface( _allocator )
        , m_provider( _provider )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Source::~Source()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    const SourceProviderInterfacePtr & Source::getSourceProvider() const
    {
        return m_provider;
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::addNode( const NodeInterfacePtr & _task )
    {
        TranscriptorBasePtr transcriptor = Helper::makeTranscriptor<TranscriptorBase>( m_allocator, _task );

        m_provider->addTranscriptor( transcriptor );
    }
    //////////////////////////////////////////////////////////////////////////
    SourceInterfacePtr Source::makeSource()
    {
        SourceInterfacePtr source = Helper::makeSource( m_allocator );

        const SourceProviderInterfacePtr & provider = source->getSourceProvider();

        const SourceProviderInterfacePtr & current = this->getSourceProvider();
        bool skip = current->isSkip();
        provider->setSkip( skip );

        return source;
    }
    //////////////////////////////////////////////////////////////////////////
    const VectorSources & Source::addParallelTranscriptor( uint32_t _count )
    {
        VectorSources sources{StlAllocator<SourceInterfacePtr>( m_allocator )};
        this->makeSources_( sources, _count );

        TranscriptorParallelPtr transcriptor = Helper::makeTranscriptor<TranscriptorParallel>( m_allocator, std::move( sources ) );

        m_provider->addTranscriptor( transcriptor );

        const VectorSources & sources_parallel = transcriptor->getSources();

        return sources_parallel;
    }
    //////////////////////////////////////////////////////////////////////////
    const VectorSources & Source::addRaceTranscriptor( uint32_t _count )
    {
        VectorSources sources{StlAllocator<SourceInterfacePtr>( m_allocator )};
        this->makeSources_( sources, _count );

        TranscriptorRacePtr transcriptor = Helper::makeTranscriptor<TranscriptorRace>( m_allocator, std::move( sources ) );

        m_provider->addTranscriptor( transcriptor );

        const VectorSources & sources_race = transcriptor->getSources();

        return sources_race;
    }
    //////////////////////////////////////////////////////////////////////////
    void Source::makeSources_( VectorSources & _sources, uint32_t _count )
    {
        _sources.resize( _count );

        for( SourceInterfacePtr & source : _sources )
        {
            source = this->makeSource();
        }
    }
    //////////////////////////////////////////////////////////////////////////
}