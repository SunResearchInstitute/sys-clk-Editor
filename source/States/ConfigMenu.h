#pragma once
#include "StateMachine.h"

class ConfigMenu : public State
{
public:
    void Enter() override {}
    void Update(StateMachine &, u64) override;
    void Exit() override {}

    std::string Name() override { return "config"; }

    ~ConfigMenu() override {}

private:
    int selection = 0;
};
