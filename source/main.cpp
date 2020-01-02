#include <switch.h>
#include "Utils.h"
#include "menu.h"
#include <stdio.h>
#include <filesystem>
#include <sys/stat.h>

#include "States/StateMachine.h"
#include "States/States.h"

using namespace std;

int main(int argc, char **argv)
{
    StateMachine stateMachine;

    stateMachine.states.push_back(new MainMenu());
    stateMachine.states.push_back(new TitleMenu());
    stateMachine.states.push_back(new ConfigMenu());
    stateMachine.states.push_back(new CPUMenu());
    stateMachine.states.push_back(new GPUMenu());
    stateMachine.states.push_back(new MEMMenu());
    stateMachine.states.push_back(new ErrorMenu());

    if (R_FAILED(ErrorMenu::error))
        stateMachine.PushState("error");
    else
    {
        Utils::titles = Utils::getAllTitles();
        if (R_FAILED(ErrorMenu::error))
            stateMachine.PushState("error");
        else
            stateMachine.PushState("main");
    }

    //Initialize console. Using NULL as the second argument tells the console library to use the internal console structure as current one.
    //this is C++ we should use nullptr instead
    consoleInit(nullptr);

    //SimpleIniParser May or may not need the file to exist so we will create it anyways
    mkdir(CONFIGDIR, 0777);
    fclose(fopen(CONFIG_INI, "a"));

    while (appletMainLoop())
    {
        u64 kDown = Utils::GetControllerInputs();
        if (kDown & KEY_PLUS || (kDown & KEY_B && stateMachine.currentState->Name() == "main"))
            break;

        stateMachine.Update(kDown);
        consoleUpdate(nullptr);
    }

    consoleExit(nullptr);
    return 0;
}

extern "C"
{
    void userAppInit(void)
    {
        Result rc;
        rc = pmshellInitialize();
        if (R_FAILED(rc))
            goto error;
        rc = pmdmntInitialize();
        if (R_FAILED(rc))
            goto error;
        rc = nsInitialize();

    error:
        ErrorMenu::error = rc;
    }

    void userAppExit(void)
    {
        nsExit();
        pmdmntExit();
        pmshellExit();
    }
}
