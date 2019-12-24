#include "ConfigMenu.h"


void ConfigMenu::Update(StateMachine &stateMachine, u64 kDown)
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
    long int size = ConfigItems.size();
    if (selection < 0)
        selection = size - 1;
    if (size <= selection)
        selection = 0;

    if (kDown & KEY_A)
    {
        Utils::configSelected = selection;
        switch (selection)
        {
        //CPU
        case 0:
        case 3:
        case 6:
        case 9:
        case 12:
            //TODO: reimplement this with state machine
            stateMachine.PushState("cpu");
            break;
        //GPU
        case 1:
        case 4:
        case 7:
        case 10:
        case 13:
            //TODO: reimplement this with state machine
            stateMachine.PushState("gpu");
            break;
        //MEM
        case 2:
        case 5:
        case 8:
        case 11:
        case 14:
            //TODO: reimplement this with state machine
            stateMachine.PushState("mem");
            break;
        case 15:
            //TODO: reimplement this with state machine
            stateMachine.PushState("title");
            break;
        default:
            break;
        }
    }

    Utils::printConfig(ConfigItems, selection);

    if (kDown & KEY_B)
    {
        //TODO: reimplement this with state machine
        stateMachine.PushState("title");
    }
}