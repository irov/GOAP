/*
* Copyright (C) 2017-2018, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Config.h"

namespace GOAP
{
    class Visitable
    {
    public:
        virtual void visit( Visitor * _visitor ) = 0;

    protected:
        template<class T>
        static bool s_concreteVisit( T * _visited, Visitor * _visitor )
        {
            typedef ConcreteVisitor<T> ConcreteVisitorT;

            ConcreteVisitorT * ptr = dynamic_cast<ConcreteVisitorT*>(_visitor);

            if( ptr == nullptr )
            {
                return false;
            }

            ptr->accept( _visited );

            return true;
        }
    };

#define DECLARE_VISITABLE_BASE() \
public:\
    void visit( Visitor * _visitor ) override { Visitable::s_concreteVisit(this, _visitor); }\
protected:

#define DECLARE_VISITABLE( Base ) \
public:\
    void visit( Visitor * _visitor ) override { if( Visitable::s_concreteVisit(this, _visitor) == false ){ Base::visit(_visitor); } }\
protected:
}