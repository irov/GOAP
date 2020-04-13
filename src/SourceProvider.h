/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/SourceProviderInterface.h"
#include "GOAP/Vector.h"
#include "GOAP/Allocator.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class NodeInterface> NodeInterfacePtr;
    typedef IntrusivePtr<class TaskInterface> TaskInterfacePtr;
    typedef IntrusivePtr<class ChainInterface> ChainInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    class SourceProvider
        : public SourceProviderInterface
    {
    public:
        SourceProvider();
        ~SourceProvider() override;

    public:
        void setSkip( bool _skip ) override;
        bool isSkip() const override;

    public:
        bool empty() const override;

    public:
        void addTranscriptor( const TranscriptorInterfacePtr & _transcriptor ) override;

    public:
        NodeInterfacePtr parse( const ChainInterfacePtr & _chain, const NodeInterfacePtr & _task ) override;

    protected:
        typedef Vector<TranscriptorInterfacePtr> VectorTranscriptors;
        VectorTranscriptors m_transcriptors;

        bool m_skip;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<SourceProvider> SourceProviderPtr;
    //////////////////////////////////////////////////////////////////////////
}