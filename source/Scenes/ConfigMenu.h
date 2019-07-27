#pragma once
#include "scene.h"

class ConfigMenu : public Scene
{
public:
    virtual void Display(u64);
    virtual ~ConfigMenu();
    ConfigMenu();
};