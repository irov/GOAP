/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "Source.h"

#include "SourceProvider.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    Source::Source( const KernelInterfacePtr & _kernel, const SourceProviderInterfacePtr & _provider )
        : SourceBase( _kernel, _provider )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Source::~Source()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    SourceInterfacePtr Source::_makeSource()
    {
        SourceInterfacePtr source = m_kernel->makeSource();

        return source;
    }
    //////////////////////////////////////////////////////////////////////////
}