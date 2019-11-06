/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/SourceProviderInterface.h"
#include "GOAP/Vector.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class Node> NodePtr;
    typedef IntrusivePtr<class TaskInterface> TaskInterfacePtr;
    typedef IntrusivePtr<class Chain> ChainPtr;
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
        NodeInterfacePtr parse( const ChainPtr & _chain, const NodeInterfacePtr & _task ) override;

    protected:
        typedef Vector<TranscriptorInterfacePtr> VectorTranscriptors;
        VectorTranscriptors m_transcriptors;

        bool m_skip;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<SourceProvider> SourceProviderPtr;
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        SourceProviderPtr makeSourceProvider();
    }
}