#pragma once

#include "GOAP/TaskInterface.h"

class TaskPrint
    : public GOAP::TaskInterface
{
public:
    TaskPrint( GOAP::Allocator * _allocator, const char * _format, ... );
    ~TaskPrint() override;

protected:
    bool _onRun( GOAP::NodeInterface * _node ) override;

protected:
    char m_message[1024];
};