#pragma once

#include "GOAP/Task.h"

class TaskPrint
    : public GOAP::Task
{
public:
    TaskPrint( const char * _format, ... );
    ~TaskPrint() override;

protected:
    bool _onRun() override;

protected:
    char m_message[1024];
};