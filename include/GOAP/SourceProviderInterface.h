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
    typedef IntrusivePtr<class NodeInterface> NodeInterfacePtr;
    typedef IntrusivePtr<class TranscriptorInterface> TranscriptorInterfacePtr;
    typedef IntrusivePtr<class ChainInterface> ChainInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    class SourceProviderInterface
        : public Factorable
    {
    public:
        virtual void setSkip( bool _skip ) = 0;
        virtual bool isSkip() const = 0;

    public:
        virtual bool empty() const = 0;

    public:
        virtual void addTranscriptor( const TranscriptorInterfacePtr & _transcriptor ) = 0;

    public:
        virtual NodeInterfacePtr parse( const ChainInterfacePtr & _chain, const NodeInterfacePtr & _task ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<SourceProviderInterface> SourceProviderInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
}