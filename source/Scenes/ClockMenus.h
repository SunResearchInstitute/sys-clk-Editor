#pragma once
#include "scene.h"

class RAMMenu : public Scene
{
public:
    virtual void Display(u64);
    virtual ~RAMMenu();
    RAMMenu();
};

class GPUMenu : public Scene
{
public:
    virtual void Display(u64);
    virtual ~GPUMenu();
    GPUMenu();
};

class CPUMenu : public Scene
{
public:
    virtual void Display(u64);
    virtual ~CPUMenu();
    CPUMenu();
};