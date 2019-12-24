#pragma once
#include "StateMachine.h"

class MEMMenu : public State
{
public:
    virtual void Enter() override {}
    virtual void Update(StateMachine &, u64) override;
    virtual void Exit() override {}

    virtual std::string Name() override { return "mem"; }

    virtual ~MEMMenu() override {}

private:
    int selection = 0;
};

class GPUMenu : public State
{
public:
    virtual void Enter() override {}
    virtual void Update(StateMachine &, u64) override;
    virtual void Exit() override {}

    virtual std::string Name() override { return "gpu"; }

    virtual ~GPUMenu() override {}

private:
    int selection = 0;
};

class CPUMenu : public State
{
public:
    virtual void Enter() override {}
    virtual void Update(StateMachine &, u64) override;
    virtual void Exit() override {}

    virtual std::string Name() override { return "cpu"; }

    virtual ~CPUMenu() override {}

private:
    int selection = 0;
};