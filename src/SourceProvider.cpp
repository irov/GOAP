/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "SourceProvider.h"

#include "GOAP/TranscriptorInterface.h"
#include "GOAP/NodeInterface.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    SourceProvider::SourceProvider( Allocator * _allocator )
        : SourceProviderInterface( _allocator )
        , m_skip( false )
        , m_transcriptors( StlAllocator<TranscriptorInterfacePtr>( _allocator ) )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    SourceProvider::~SourceProvider()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceProvider::setSkip( bool _skip )
    {
        m_skip = _skip;
    }
    //////////////////////////////////////////////////////////////////////////
    bool SourceProvider::isSkip() const
    {
        return m_skip;
    }
    //////////////////////////////////////////////////////////////////////////
    bool SourceProvider::empty() const
    {
        bool result = m_transcriptors.empty();

        return result;
    }
    //////////////////////////////////////////////////////////////////////////
    void SourceProvider::addTranscriptor( const TranscriptorInterfacePtr & _transcriptor )
    {
        m_transcriptors.emplace_back( _transcriptor );
    }
    //////////////////////////////////////////////////////////////////////////
    NodeInterfacePtr SourceProvider::parse( const ChainInterfacePtr & _chain, const NodeInterfacePtr & _task )
    {
        NodeInterfacePtr current_task = _task;

        for( const TranscriptorInterfacePtr & description : m_transcriptors )
        {
            NodeInterfacePtr last_task = description->generate( _chain, current_task );

            current_task = last_task;
        }

        m_transcriptors.clear();

        return current_task;
    }
}