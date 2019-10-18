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
    typedef IntrusivePtr<class Node> NodePtr;
    typedef IntrusivePtr<class TranscriptorInterface> TranscriptorInterfacePtr;
    typedef IntrusivePtr<class Chain> ChainPtr;
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
        virtual NodePtr parse( const ChainPtr & _chain, const NodePtr & _task ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<SourceProviderInterface> SourceProviderInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
}