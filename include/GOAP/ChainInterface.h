/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Factorable.h"
#include "GOAP/IntrusivePtr.h"
#include "GOAP/ChainProvider.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class SourceInterface> SourceInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    class ChainInterface
        : public Factorable
    {
    public:
        template<class F>
        void setCallback( F _f )
        {
            ChainProviderPtr untilChainProvider = Helper::makeChainProvider( _f );

            this->setCallbackProvider( untilChainProvider );
        }

        virtual void setCallbackProvider( const ChainProviderPtr & _cb ) = 0;
        virtual const ChainProviderPtr & getCallbackProvider() const = 0;

    public:
        virtual const SourceInterfacePtr & getSource() const = 0;

    public:
        virtual bool run() = 0;
        virtual void skip() = 0;
        virtual void cancel() = 0;

    public:
        virtual bool isComplete() const = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<ChainInterface> ChainInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
}