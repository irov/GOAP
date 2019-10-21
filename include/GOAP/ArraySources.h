/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Array.h"
#include "GOAP/IntrusivePtr.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class SourceInterface> SourceInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    template<size_t Count>
    using ArraySources = Array<SourceInterfacePtr, Count>;
    //////////////////////////////////////////////////////////////////////////
    template<class Type, size_t Count>
    using ArrayTypeSources = Array<IntrusivePtr<Type>, Count>;
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        template <class T, size_t N, size_t... Is>
        ArrayTypeSources<T, N> ArraySourcesCast( const ArraySources<N> & arr, std::index_sequence<Is...> )
        {
            return ArrayTypeSources<T, N>{IntrusivePtr<T>{arr[Is]}...};
        }

        template <class T, size_t N>
        ArrayTypeSources<T, N> ArraySourcesCast( const ArraySources<N> & arr )
        {
            return ArraySourcesCast<T>( arr, std::make_index_sequence<N>{} );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}
