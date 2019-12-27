#pragma once
#include "StateMachine.h"

class MEMMenu : public State
{
public:
    virtual void Update(StateMachine &, u64);
    virtual std::string Name() { return "mem"; }

private:
    int selection = 0;
};

class GPUMenu : public State
{
public:
    virtual void Update(StateMachine &, u64);
    virtual std::string Name() { return "gpu"; }

private:
    int selection = 0;
};

class CPUMenu : public State
{
public:
    virtual void Update(StateMachine &, u64);
    virtual std::string Name() { return "cpu"; }

private:
    int selection = 0;
};
