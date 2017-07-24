/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#   pragma once

#	include "GOAP/Config.h"

namespace GOAP
{
    class Factorable
        : public IntrusiveBase<Factorable>
    {
    public:
        Factorable(){};
        virtual ~Factorable(){};
    };
}