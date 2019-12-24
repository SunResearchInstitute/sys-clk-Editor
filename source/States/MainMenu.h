#pragma once
#include "StateMachine.h"

class MainMenu : public State
{
public:
    virtual void Enter() override;
    virtual void Update(StateMachine &, u64) override;
    virtual void Exit() override {}

    virtual std::string Name() override { return "main"; }

    virtual ~MainMenu() override {}

private:
    int selection = 0;
    std::vector<std::string> menuItems{"Games", "PlaceHldr", "PlaceHldr", "Delete config"};
    void UpdateItems();
};