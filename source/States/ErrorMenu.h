#pragma once
#include "StateMachine.h"

class ErrorMenu : public State
{
    void Enter() override {}
    void Update(StateMachine &, u64) override;
    void Exit() override {}

    std::string Name() override { return "error"; }

    

    ~ErrorMenu() override {}

public:
    static Result error;
};
