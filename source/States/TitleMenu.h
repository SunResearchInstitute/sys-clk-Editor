#pragma once
#include "StateMachine.h"

class TitleMenu : public State
{
public:
    void Enter() override {}
    void Update(StateMachine &, u64) override;
    void Exit() override {}

    std::string Name() override { return "title"; }

    ~TitleMenu() override {}

private:
    int selection = 0;
};
