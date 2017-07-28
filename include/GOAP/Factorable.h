/*
* Copyright (C) 2017, Levchenko Yuriy <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#   pragma once

#	include "GOAP/IntrusiveBase.h"

namespace GOAP
{
    namespace Detail
    {
        class Factorable
            : virtual public IntrusiveBase
        {
        public:
            Factorable();
            virtual ~Factorable();

        protected:
            void destroy() override;
        };
    }
}