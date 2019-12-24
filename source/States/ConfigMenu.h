#pragma once
#include "StateMachine.h"

class ConfigMenu : public State
{
public:
    virtual void Enter() override {}
    virtual void Update(StateMachine &, u64) override;
    virtual void Exit() override {}

    virtual std::string Name() override { return "config"; }

    virtual ~ConfigMenu() override {}

private:
    int selection = 0;
};