#include "ConfigMenu.h"

ConfigMenu::ConfigMenu() {}
ConfigMenu::~ConfigMenu() {}
void ConfigMenu::Display(u64 kDown)
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

    // check for under/overflow
    long int size = ConfigItems.size();
    if (selection < 0)
        selection = size - 1;
    if (size <= selection)
        selection = 0;

    if (kDown & KEY_A)
    {
        printf(CONSOLE_ESC(2J));
        configSelected = selection;
        switch (selection)
        {
        //CPU
        case 0:
        case 3:
        case 6:
        case 9:
        case 12:
            selection = 0;
            scene = 3;
            Utils::printItems(CPUClocks, "CPU Clocks");
            break;
        //GPU
        case 1:
        case 4:
        case 7:
        case 10:
        case 13:
            selection = 0;
            scene = 4;
            Utils::printItems(GPUClocks, "GPU Clocks");
            break;
        //MEM
        case 2:
        case 5:
        case 8:
        case 11:
        case 14:
            selection = 0;
            scene = 5;
            Utils::printItems(MEMClocks, "MEM Clocks");
            break;
        case 15:
            Utils::resetConfig();
            scene = 1;
            selection = gameSelected - (title_page * max_title_items);
            Utils::printTitles();
            break;
        default:
            break;
        }
    }

    if (needsRefresh)
    {
        printf(CONSOLE_ESC(2J));
        Utils::printConfig(ConfigItems);
    }

    if (kDown & KEY_B)
    {
        printf(CONSOLE_ESC(2J));
        scene = 1;
        selection = gameSelected - (title_page * max_title_items);
        Utils::printTitles();
    }
}