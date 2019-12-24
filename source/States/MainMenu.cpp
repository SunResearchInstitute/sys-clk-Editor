#include "MainMenu.h"
#include <filesystem>
#include <sys/stat.h>

using namespace std;

void MainMenu::Enter()
{
    UpdateItems();
}

void MainMenu::Update(StateMachine &stateMachine, u64 kDown)
{
    if (kDown & KEY_UP)
    {
        selection--;
    }
    if (kDown & KEY_DOWN)
    {
        selection++;
    }

    // check for under/overflow
    long int size = menuItems.size();
    if (selection < 0)
        selection = size - 1;
    if (size <= selection)
        selection = 0;

    if (kDown & KEY_A)
    {
        switch (selection)
        {
        case 0:
            stateMachine.PushState("title");
            return;
        case 1:
        {
            if (!filesystem::exists(LOG))
                fclose(fopen(LOG, "w"));
            else
                remove(LOG);

            break;
        }
        case 3:
            fclose(fopen(CONFIG, "w"));
            break;
        case 2:
        {
            ClkState clkState = Utils::getClkState();
            if (clkState == ClkState::Enabled)
            {
                if (R_SUCCEEDED(pmshellTerminateProgram(sysClkTid)))
                {
                    remove(BOOT2FLAG);
                }
            }
            else if (clkState == ClkState::Disabled)
            {
                NcmProgramLocation programLocation{
                    .program_id = sysClkTid,
                    .storageID = NcmStorageId_None,
                };
                u64 pid;
                if (R_SUCCEEDED(pmshellLaunchProgram(0, &programLocation, &pid)))
                {
                    mkdir(FLAGSDIR, 0777);
                    fclose(fopen(BOOT2FLAG, "w"));
                }
            }
            break;
        }
        }
    }

    UpdateItems();
}

void MainMenu::UpdateItems()
{
    if (filesystem::exists(LOG))
        menuItems[1] = "Toggle sys-clk Logging: Enabled";
    else
        menuItems[1] = "Toggle sys-clk Logging: Disabled";

    switch (Utils::getClkState())
    {
    case ClkState::NotFound:
        menuItems[2] = CONSOLE_RED "!!! sys-clk was not found!";
        break;
    case ClkState::Error:
        menuItems[2] = CONSOLE_RED "!!! Could not retreive the state of sys-clk!";
        break;
    case ClkState::Enabled:
        menuItems[2] = "sys-clk is enabled!";
        break;
    case ClkState::Disabled:
        menuItems[2] = "sys-clk is disabled!";
        break;
    }

    if (filesystem::exists(CONFIG) && filesystem::file_size(CONFIG) > 0)
        menuItems[3] = "Clear config";
    else
        menuItems[3] = "Config is cleared";

    consoleClear();
    Utils::printItems(menuItems, "Main Menu", selection);
}