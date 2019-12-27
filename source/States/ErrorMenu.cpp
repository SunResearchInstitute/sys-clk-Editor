#include "ErrorMenu.h"
#include "../Utils.h"
#include <cstring>

Result ErrorMenu::error = 0;

void ErrorMenu::Update(StateMachine &stateMachine, u64 kDown)
{
    consoleClear();
    printf(CONSOLE_RED "Error: 0x%x\n", ErrorMenu::error);
    printf(CONSOLE_RED "Press `+` to exit!\n");
}