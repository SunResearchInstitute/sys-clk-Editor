#pragma once
#include "StateMachine.h"

class ErrorMenu : public State
{
    virtual void Enter() override {}
    virtual void Update(StateMachine &, u64) override;
    virtual void Exit() override {}

    virtual std::string Name() override { return "error"; }

    

    virtual ~ErrorMenu() override {}

public:
    static Result error;
};