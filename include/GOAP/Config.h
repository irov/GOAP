/*
* Copyright (C) 2017, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#   pragma once

#   include <stdint.h>

#   include "GOAP/IntrusivePtr.h"
#   include "GOAP/Factorable.h"
#   include "GOAP/Visitor.h"

namespace GOAP
{
    template<class T>
    using IntrusivePtr = Detail::IntrusivePtr<T>;

    using Factorable = Detail::Factorable;
    using Visitor = Detail::Visitor;
}
