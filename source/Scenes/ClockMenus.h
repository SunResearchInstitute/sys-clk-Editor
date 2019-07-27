#pragma once
#include "scene.h"

class RAMMenu : public Scene
{
public:
    virtual void Display(u64);
    RAMMenu();
};

class GPUMenu : public Scene
{
public:
    virtual void Display(u64);
    GPUMenu();
};

class CPUMenu : public Scene
{

public:
    virtual void Display(u64);
    CPUMenu();
};