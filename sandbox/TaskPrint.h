#pragma once

#include "GOAP/Task.h"

#include <map>
#include <string>

class TaskPrint
    : public GOAP::Task
{
public:
    TaskPrint( const std::string & _message );
    ~TaskPrint() override;

protected:
    bool _onRun() override;

protected:
    std::string m_message;
};