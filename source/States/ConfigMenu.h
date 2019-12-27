#pragma once
#include "StateMachine.h"

class ConfigMenu : public State
{
public:
    virtual void Update(StateMachine &, u64);
    virtual std::string Name() { return "config"; }

private:
    int selection = 0;
};
