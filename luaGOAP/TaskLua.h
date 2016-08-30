#	pragma once

#	include "GOAP/Task.h"

#	include "lua.hpp"

class TaskLua
	: public GOAP::Task
{
public:
	TaskLua( lua_State * L, int _ref );
	~TaskLua();
	
protected:
	bool onInitialize() override;
	void onFinalize() override;

protected:
	bool onValidate() const override;
	bool onCheck() const override;
	bool onRun() override;
	bool onSkipable() const override;
	void onSkipNoSkiped() override;
	bool onSkipBlock() override;
	void onComplete() override;
	void onSkip() override;
	void onCancel() override;
	void onFinally() override;
	bool onCheckRun() const override;
	bool onCheckSkip() const override;

protected:
	bool callMethod_( const char * _method, int _return = 0 ) const;
	bool askMethod_( const char * _method, bool _default ) const;

protected:
	lua_State * m_L;
	int m_ref;
};

typedef GOAP::IntrusivePtr<TaskLua> TaskLuaPtr;