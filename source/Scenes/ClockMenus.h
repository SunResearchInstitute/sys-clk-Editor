#pragma once
#include "scene.h"

class MEMMenu : public Scene
{
public:
    virtual void Display(u64);
    virtual ~MEMMenu();
    MEMMenu();
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