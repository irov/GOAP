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
    typedef IntrusivePtr<class ChainInterface> ChainInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    class TranscriptorInterface
        : public Factorable
    {
    public:
        virtual NodeInterfacePtr generate( const ChainInterfacePtr & _chain, const NodeInterfacePtr & _task ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TranscriptorInterface> TranscriptorInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        template<class T, class ... Args>
        IntrusivePtr<T> makeTranscriptor( Args && ... _args )
        {
            return IntrusivePtr<T>::from( new T( std::forward<Args &&>( _args ) ... ) );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}