/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#	include "GOAP/Factorable.h"

namespace GOAP
{
    namespace Detail
    {
        //////////////////////////////////////////////////////////////////////////
        Factorable::Factorable()
        {
        }
        //////////////////////////////////////////////////////////////////////////
        Factorable::~Factorable()
        {
        }
        //////////////////////////////////////////////////////////////////////////
        void Factorable::destroy()
        {
            delete this;
        }
    }
}