#pragma once
#include "scene.h"

class ErrorMenu : public Scene
{
public:
    virtual void Display(u64);
    virtual ~ErrorMenu();
    ErrorMenu();
};
