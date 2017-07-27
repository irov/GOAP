#   pragma once

#   include "GOAP/Config.h"

namespace GOAP
{
    typedef IntrusivePtr<class Task> TaskPtr;

    class Factory
        : public Factorable
    {
    public:
        virtual TaskPtr generate( const TypeId & _type, const Params & _params ) = 0;
    };
    
    typedef IntrusivePtr<Factory> FactoryPtr;
}