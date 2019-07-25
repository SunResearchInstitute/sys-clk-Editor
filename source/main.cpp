#include <switch.h>

#include "menu.h"
#include <stdio.h>
#include <filesystem>

using namespace std;

int main(int argc, char **argv)
{
    //Initialize console. Using NULL as the second argument tells the console library to use the internal console structure as current one.
    //this is C++ we should use nullptr instead
    consoleInit(nullptr);

    if (filesystem::exists(configFile))
        menuMainLoop();
    else
    {
        printf("Failed to open Sys-clk Config! press + to exit.");
        while (appletMainLoop())
        {
            //Scan all the inputs. This should be done once for each frame
            hidScanInput();
            //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
            u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

            if (kDown & KEY_PLUS)
                break;

            consoleUpdate(nullptr);
        }
    }
    consoleExit(nullptr);
    return 0;
}
