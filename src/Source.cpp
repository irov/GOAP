/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#include "GOAP/Source.h"
#include "GOAP/SourceProvider.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    SourceInterfacePtr Source::_makeSource()
    {
        return Helper::makeSource();
    }
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        SourcePtr makeSource()
        {
            SourceProviderPtr provider = Helper::makeSourceProvider();

            return SourcePtr::from( new Source( provider ) );
        }
    }
}