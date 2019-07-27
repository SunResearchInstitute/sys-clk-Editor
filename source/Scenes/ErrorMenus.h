#pragma once
#include "scene.h"

class nsFailedMenu : public Scene
{
public:
    virtual void Display(u64);
    nsFailedMenu();
};

class nsRecordFailedMenu : public Scene
{
public:
    virtual void Display(u64);
    nsRecordFailedMenu();
};