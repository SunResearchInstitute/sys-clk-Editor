#include "MainMenu.h"
#include <filesystem>

using namespace std;

MainMenu::MainMenu() {}
MainMenu::~MainMenu() {}
void MainMenu::Display(u64 kDown)
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
    long int size = firstMenuItems.size();
    if (selection < 0)
        selection = size - 1;
    if (size <= selection)
        selection = 0;

    if (kDown & KEY_A)
    {
        switch (selection)
        {
        case 0:
            scene = 1;
            selection = 0;
            printf(CONSOLE_ESC(2J));
            Utils::printTitles();
            break;
        case 1:
        {
            if (!filesystem::exists(logFlag))
            {
                fclose(fopen(logFlag.c_str(), "w"));
                firstMenuItems[1] = "Toggle sys-clk Logging: Enabled";
            }
            else
            {
                remove(logFlag.c_str());
                firstMenuItems[1] = "Toggle sys-clk Logging: Disabled";
            }
            needsRefresh = true;
            break;
        }
        case 4:
            fclose(fopen(configFile.c_str(), "w"));
            scene = 1;
            selection = 0;
            printf(CONSOLE_ESC(2J));
            Utils::printTitles();
            break;
        case 2:
        {
            if (Utils::isClkActive())
            {
                if (R_SUCCEEDED(pmshellTerminateProcessByTitleId(sysClkTid)))
                {
                    firstMenuItems[2] = "sys-clk is disabled!";
                    remove(boot2Flag.c_str());
                }
            }
            else
            {
                u64 pid;
                if (R_SUCCEEDED(pmshellLaunchProcess(0, sysClkTid, FsStorageId_None, &pid)))
                {
                    firstMenuItems[2] = "sys-clk is enabled!";
                    fclose(fopen(boot2Flag.c_str(), "w"));
                }
            }
            needsRefresh = true;
            break;
        }
        }
    }

    if (needsRefresh)
    {
        printf(CONSOLE_ESC(2J));
        Utils::printItems(firstMenuItems, "Main Menu");
    }
}