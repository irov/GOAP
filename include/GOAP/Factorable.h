/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Macros.h"
#include "GOAP/IntrusiveBase.h"

namespace GOAP
{
    namespace Detail
    {
        class Factorable
            : public IntrusiveBase
        {
        public:
            Factorable();
            virtual ~Factorable();

        protected:
            void destroy() override;
        };
    }
}