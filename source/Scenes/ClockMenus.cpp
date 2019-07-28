#include "ClockMenus.h"

CPUMenu::CPUMenu() {}
CPUMenu::~CPUMenu() {}
void CPUMenu::Display(u64 kDown)
{
    bool needsRefresh = false;
    if (kDown & KEY_UP)
    {
        selection--;
        needsRefresh = true;
    }

    if (kDown & KEY_DOWN)
    {
        selection++;
        needsRefresh = true;
    }

    long int size = CPUClocks.size();
    if (selection < 0)
        selection = size - 1;
    if (size <= selection)
        selection = 0;

    if (kDown & KEY_A)
    {
        ChangeConfiguration(CPUClocks);
        scene = 2;
        selection = configSelected;
        printf(CONSOLE_ESC(2J));
        printConfig(ConfigItems);
    }

    if (needsRefresh)
    {
        printf(CONSOLE_ESC(2J));
        printItems(CPUClocks, "CPU Clocks");
    }

    if (kDown & KEY_B)
    {
        printf(CONSOLE_ESC(2J));
        scene = 2;
        selection = configSelected;
        printConfig(ConfigItems);
    }
}

GPUMenu::GPUMenu() {}
GPUMenu::~GPUMenu() {}
void GPUMenu::Display(u64 kDown)
{
    bool needsRefresh = false;
    if (kDown & KEY_UP)
    {
        selection--;
        needsRefresh = true;
    }

    if (kDown & KEY_DOWN)
    {
        selection++;
        needsRefresh = true;
    }

    long int size = GPUClocks.size();
    if (selection < 0)
        selection = size - 1;
    if (size <= selection)
        selection = 0;

    if (kDown & KEY_A)
    {
        ChangeConfiguration(GPUClocks);
        scene = 2;
        selection = configSelected;
        printf(CONSOLE_ESC(2J));
        printConfig(ConfigItems);
    }

    if (needsRefresh)
    {
        printf(CONSOLE_ESC(2J));
        printItems(GPUClocks, "GPU Clocks");
    }

    if (kDown & KEY_B)
    {
        printf(CONSOLE_ESC(2J));
        scene = 2;
        selection = configSelected;
        printConfig(ConfigItems);
    }
}

MEMMenu::MEMMenu() {}
MEMMenu::~MEMMenu() {}
void MEMMenu::Display(u64 kDown)
{
    bool needsRefresh = false;
    if (kDown & KEY_UP)
    {
        selection--;
        needsRefresh = true;
    }

    if (kDown & KEY_DOWN)
    {
        selection++;
        needsRefresh = true;
    }

    long int size = MEMClocks.size();
    if (selection < 0)
        selection = size - 1;
    if (size <= selection)
        selection = 0;

    if (kDown & KEY_A)
    {
        ChangeConfiguration(MEMClocks);
        scene = 2;
        selection = configSelected;
        printf(CONSOLE_ESC(2J));
        printConfig(ConfigItems);
    }

    if (needsRefresh)
    {
        printf(CONSOLE_ESC(2J));
        printItems(MEMClocks, "MEM Clocks");
    }

    if (kDown & KEY_B)
    {
        printf(CONSOLE_ESC(2J));
        scene = 2;
        selection = configSelected;
        printConfig(ConfigItems);
    }
}