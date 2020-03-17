/*
* Copyright (C) 2017-2019, Yuriy Levchenko <irov13@mail.ru>
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#pragma once

#include "GOAP/KernelInterface.h"

#include "GOAP/Allocator.h"

namespace GOAP
{ 
    class Kernel
        : public KernelInterface
    {
    public:
        Kernel();
        ~Kernel() override;

    public:
        NodeInterfacePtr makeNode( const TaskInterfacePtr & _task ) override;
        ChainInterfacePtr makeChain( const SourceInterfacePtr & _source, const char * _file, uint32_t _line ) override;
        EventInterfacePtr makeEvent() override;
        SemaphoreInterfacePtr makeSemaphore( const EventInterfacePtr & _event, int32_t _value ) override;
        SourceProviderInterfacePtr makeSourceProvider() override;
        SourceInterfacePtr makeSource() override;
        TimerInterfacePtr makeTimer() override;
    };
}