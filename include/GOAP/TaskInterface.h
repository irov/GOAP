/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/Factorable.h"
#include "GOAP/IntrusivePtr.h"
#include "GOAP/Visitable.h"

namespace GOAP
{
    class TaskInterface
        : public Factorable
        , public Visitable
    {
        GOAP_DECLARE_VISITABLE_BASE();

    protected:
        virtual bool _onInitialize() = 0;
        virtual void _onFinalize() = 0;

    protected:
        virtual bool _onValidate() const = 0;
        virtual bool _onCheck() = 0;
        virtual bool _onRun( class NodeInterface * _node ) = 0;
        virtual bool _onSkipable() const = 0;
        virtual void _onSkipNoSkiped() = 0;
        virtual bool _onSkipBlock() = 0;
        virtual void _onComplete() = 0;
        virtual bool _onFastSkip() = 0;
        virtual void _onSkip() = 0;
        virtual void _onCancel() = 0;
        virtual void _onFinally() = 0;
        virtual bool _onCheckRun( const class NodeInterface * _node ) const = 0;
        virtual bool _onCheckSkip( const class NodeInterface * _node ) const = 0;

    protected:
        friend class Node;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskInterface> TaskInterfacePtr;
    //////////////////////////////////////////////////////////////////////////    
    namespace Helper
    {
        template<class T, class ... Args>
        IntrusivePtr<T> makeTask( Args && ... _args )
        {
            return IntrusivePtr<T>::from( new T( std::forward<Args &&>( _args ) ... ) );
        }
    }
}