/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#   pragma once

#   include <vector>
#   include <map>
#   include <string>
#   include <any>

#   include "GOAP/IntrusivePtr.h"
#   include "GOAP/Factorable.h"

namespace GOAP
{
    template<class T>
    using Vector = std::vector<T>;

    typedef std::string TypeId;
    
    typedef std::map<std::string, std::any> Params;

    template<class T>
    using IntrusivePtr = Detail::IntrusivePtr<T>;
    
    using Factorable = Detail::Factorable;
}