#   pragma once

#   include <vector>

#   include "IntrusivePtr.h"
#   include "IntrusiveBase.h"

namespace GOAP
{
    template<class T>
    using Vector = std::vector<T>;

    template<class T>
    using IntrusivePtr = Detail::IntrusivePtr<T>;

    template<class T>
    using IntrusiveBase = Detail::IntrusiveBase<T>;
}