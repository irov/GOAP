#pragma once

#include "GOAP/Task.h"

class TaskPrint
    : public GOAP::Task
{
public:
    TaskPrint( const char * _format, ... );
    ~TaskPrint() override;

protected:
    bool _onRun( GOAP::NodeInterface * _node ) override;

protected:
    char m_message[1024];
};