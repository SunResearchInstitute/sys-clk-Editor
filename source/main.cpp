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

    Result rc;
    rc = pmshellInitialize();
    if (R_FAILED(rc))
        Utils::printError("PMSHELL failed to start!");
    rc = pmdmntInitialize();
    if (R_FAILED(rc))
        Utils::printError("PMDMNT failed to start!");
    rc = nsInitialize();
    if (R_FAILED(rc))
        Utils::printError("NS failed to start!");
    else
        titles = Utils::getAllTitles();

    //SimpleIniParser May or may not need the file to exist so we will create it anyways
    if (!filesystem::exists(configFile))
        fclose(fopen(configFile.c_str(), "a"));

    menuMainLoop();

    pmshellExit();
    pmdmntExit();
    nsExit();
    
    return 0;
}
