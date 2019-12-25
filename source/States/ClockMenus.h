#pragma once
#include "StateMachine.h"

class MEMMenu : public State
{
public:
    void Enter() override {}
    void Update(StateMachine &, u64) override;
    void Exit() override {}

    std::string Name() override { return "mem"; }

    ~MEMMenu() override {}

private:
    int selection = 0;
};

class GPUMenu : public State
{
public:
    void Enter() override {}
    void Update(StateMachine &, u64) override;
    void Exit() override {}

    std::string Name() override { return "gpu"; }

    ~GPUMenu() override {}

private:
    int selection = 0;
};

class CPUMenu : public State
{
public:
    void Enter() override {}
    void Update(StateMachine &, u64) override;
    void Exit() override {}

    std::string Name() override { return "cpu"; }

    ~CPUMenu() override {}

private:
    int selection = 0;
};
