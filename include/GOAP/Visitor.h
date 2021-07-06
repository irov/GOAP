/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Config.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class Visitor
    {
    public:
        Visitor()
        {
        };

        virtual ~Visitor()
        {
        }
    };
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    class ConcreteVisitor
    {
    public:
        virtual void accept( T * _visited ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
}