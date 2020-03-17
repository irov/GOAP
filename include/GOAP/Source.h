/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/SourceInterface.h"
#include "GOAP/SourceBase.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class Source
        : public SourceBase
    {   
    public:
        Source( const KernelInterfacePtr & _kernel, const SourceProviderInterfacePtr & _provider );
        ~Source() override;

    protected:
        SourceInterfacePtr _makeSource() override;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<Source> SourcePtr;
    //////////////////////////////////////////////////////////////////////////
}