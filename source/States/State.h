#pragma once
#include "switch.h"
#include "../menu.h"
#include "../Utils.h"

class StateMachine;

class State
{
public:
    virtual void Enter() = 0;
    virtual void Update(StateMachine &stateMachine, u64 kDown) = 0;
    virtual void Exit() = 0;

    virtual std::string Name() = 0;

    virtual ~State(){};
};
