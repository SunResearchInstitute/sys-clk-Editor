#include "MainMenu.h"
#include <filesystem>
#include <sys/stat.h>

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
            if (!filesystem::exists(LOG))
            {
                fclose(fopen(LOG, "w"));
                firstMenuItems[1] = "Toggle sys-clk Logging: Enabled";
            }
            else
            {
                remove(LOG);
                firstMenuItems[1] = "Toggle sys-clk Logging: Disabled";
            }
            needsRefresh = true;
            break;
        }
        case 3:
            fclose(fopen(LOG, "w"));
            scene = 1;
            selection = 0;
            printf(CONSOLE_ESC(2J));
            Utils::printTitles();
            break;
        case 2:
        {
            if (Utils::isClkActive())
            {
                if (R_SUCCEEDED(pmshellTerminateProgram(sysClkTid)))
                {
                    firstMenuItems[2] = "sys-clk is disabled!";
                    remove(BOOT2FLAG);
                }
            }
            else
            {
                NcmProgramLocation programLocation{
                    .program_id = sysClkTid,
                    .storageID = NcmStorageId_None,
                };
                u64 pid;
                if (R_SUCCEEDED(pmshellLaunchProgram(0, &programLocation, &pid)))
                {
                    firstMenuItems[2] = "sys-clk is enabled!";
                    mkdir(FLAGFOLDER, 0777);
                    fclose(fopen(BOOT2FLAG, "w"));
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