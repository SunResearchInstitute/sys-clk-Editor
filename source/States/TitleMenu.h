#pragma once
#include "StateMachine.h"

class TitleMenu : public State
{
public:
    virtual void Update(StateMachine &, u64);
    virtual std::string Name() { return "title"; }

private:
    int selection = 0;
};
