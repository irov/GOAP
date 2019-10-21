/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/SourceProxy.h"
#include "GOAP/Vector.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class Source
        : public SourceProxy<Source>
    {   
    public:
        using SourceProxy::SourceProxy;

    protected:
        SourceInterfacePtr _makeSource() override;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<Source> SourcePtr;
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        SourcePtr makeSource();
    }
    //////////////////////////////////////////////////////////////////////////
}