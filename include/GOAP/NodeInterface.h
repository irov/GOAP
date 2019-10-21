/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/TaskInterface.h"
#include "GOAP/Vector.h"

namespace GOAP
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class NodeInterface> NodeInterfacePtr;
    typedef IntrusivePtr<class SourceProviderInterface> SourceProviderInterfacePtr;
    typedef IntrusivePtr<class SourceInterface> SourceInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    class NodeInterface
        : public Factorable
    {
    public:
        virtual SourceInterfacePtr makeSource() = 0;

    public:
        virtual void throwError() = 0;

    public:
        virtual bool isIdle() const = 0;
        virtual bool isRunning() const = 0;
        virtual bool isEnd() const = 0;
        virtual bool isSkip() const = 0;
        virtual bool isError() const = 0;

    public:
        virtual void addNext( const NodeInterfacePtr & _task ) = 0;
        virtual void removeNext( const NodeInterfacePtr & _task ) = 0;

        virtual void addPrev( const NodeInterfacePtr & _task ) = 0;
        virtual void removePrev( const NodeInterfacePtr & _task ) = 0;

    public:
        virtual bool injectSource( const SourceProviderInterfacePtr & _source ) = 0;
        virtual bool forkSource( const SourceProviderInterfacePtr & _source ) = 0;

    public:
        virtual bool run( bool _checkSkipedFalse = false ) = 0;
        virtual bool skip() = 0;

        virtual void cancel( bool _withNexts = false ) = 0;

    public:
        virtual void complete( bool _running = true, bool _skiped = false ) = 0;

    public:
        virtual bool isEmptyPrevs() const = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<NodeInterface> NodeInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
}