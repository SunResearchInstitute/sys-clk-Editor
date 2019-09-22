#include <switch.h>
#include "Utils.h"
#include "menu.h"
#include <stdio.h>
#include <filesystem>

using namespace std;

int main(int argc, char **argv)
{
    //Initialize console. Using NULL as the second argument tells the console library to use the internal console structure as current one.
    //this is C++ we should use nullptr instead
    consoleInit(nullptr);
    //SimpleIniParser May or may not need the file to exist so we will create it anyways
    if (!filesystem::exists(configFile))
    {
        Utils::Log("configFile did not exist, created!");
        fclose(fopen(configFile.c_str(), "w"));
    }
    Utils::Log("Entering main loop!");
    menuMainLoop();
    Utils::Log("Exit!");
    consoleExit(nullptr);
    return 0;
}
