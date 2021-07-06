/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/NodeInterface.h"

#include "GOAP/Visitable.h"
#include "GOAP/Allocator.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    class TaskInterface
        : public Factorable
        , public Visitable
    {
        GOAP_DECLARE_VISITABLE_BASE();

    public:
        TaskInterface( Allocator * _allocator )
            : Factorable( _allocator )
        {
        }

    protected:
        virtual bool _onInitialize()
        {
            return true;
        };

        virtual void _onFinalize()
        {
            //Empty
        }

    protected:
        virtual bool _onValidate() const
        {
            return true;
        }

        virtual bool _onCheck()
        {
            return true;
        }

        virtual bool _onRun( NodeInterface * _node )
        {
            GOAP_UNUSED( _node );

            return true;
        }

        virtual bool _onSkipable() const
        {
            return true;
        }

        virtual void _onSkipNoSkiped()
        {
            //Empty
        }

        virtual bool _onSkipBlock()
        {
            return false;
        }

        virtual void _onComplete()
        {
            //Empty
        };

        virtual bool _onFastSkip()
        {
            return false;
        }

        virtual void _onSkip()
        {
            //Empty
        }

        virtual void _onCancel()
        {
            //Empty
        }

        virtual void _onFinally()
        {
            //Empty
        }

        virtual bool _onCheckRun( const NodeInterface * _node ) const
        {
            bool result = _node->isEmptyPrevs();

            return result;
        }

        virtual bool _onCheckSkip( const NodeInterface * _node ) const
        {
            bool result = _node->isEmptyPrevs();

            return result;
        }

    protected:
        friend class Node;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TaskInterface> TaskInterfacePtr;
    //////////////////////////////////////////////////////////////////////////    
    namespace Helper
    {
        template<class T, class ... Args>
        IntrusivePtr<T> makeTask( Allocator * _allocator, Args && ... _args )
        {
            T * task = _allocator->allocateT<T>( std::forward<Args>( _args ) ... );

            return IntrusivePtr<T>::from( task );
        }
    }
    //////////////////////////////////////////////////////////////////////////
}