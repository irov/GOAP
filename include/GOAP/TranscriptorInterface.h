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
    typedef IntrusivePtr<class Chain> ChainPtr;
    //////////////////////////////////////////////////////////////////////////
    class TranscriptorInterface
        : public Factorable
    {
    public:
        virtual NodePtr generate( const ChainPtr & _chain, const NodePtr & _task ) = 0;
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