/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/IfProvider.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class IfProviderMember
        : public IfProvider
    {
    public:
        explicit IfProviderMember( bool * _member );

    public:
        bool onIf() override;

    protected:
        bool * m_member;
    };
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        IfProviderPtr makeIfProviderMember( bool * _member );
    }
    //////////////////////////////////////////////////////////////////////////
}