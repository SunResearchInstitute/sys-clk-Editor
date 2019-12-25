#pragma once
#include "StateMachine.h"

class MainMenu : public State
{
public:
    void Enter() override;
    void Update(StateMachine &, u64) override;
    void Exit() override {}

    std::string Name() override { return "main"; }

    ~MainMenu() override {}

private:
    int selection = 0;
    std::vector<std::string> menuItems{"Games", "PlaceHldr", "PlaceHldr", "Delete config"};
    void UpdateItems();
};
