/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#   pragma once

#   include <vector>

#   include "GOAP/IntrusivePtr.h"
#   include "GOAP/Factorable.h"

namespace GOAP
{
    template<class T>
    using Vector = std::vector<T>;

    template<class T>
    using IntrusivePtr = Detail::IntrusivePtr<T>;
    
    using Factorable = Detail::Factorable;
}
