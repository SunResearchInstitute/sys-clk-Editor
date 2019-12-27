#pragma once
#include "StateMachine.h"

class MainMenu : public State
{
public:
    virtual void Enter();
    virtual void Update(StateMachine &, u64);
    virtual std::string Name() { return "main"; }

    ~MainMenu() override {}

private:
    int selection = 0;
    std::vector<std::string> menuItems{"Games", "PlaceHldr", "PlaceHldr", "Delete config"};
    void UpdateItems();
};
