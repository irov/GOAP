#	pragma once

#	include "GOAP/Task.h"

#	include <map>

class TaskPrint
	: public GOAP::Task
{
public:
	TaskPrint( const std::string & _message );
	~TaskPrint();

protected:
	bool _onRun() override;

protected:
	std::string m_message;
};