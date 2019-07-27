#include "ErrorMenus.h"
nsFailedMenu::nsFailedMenu() {}
void nsFailedMenu::Display(u64 kDown)
{
    printf(CONSOLE_ESC(2J));
    printf(CONSOLE_RED "nsInitialize Failed!, click `+` to exit!\n");
}

nsRecordFailedMenu::nsRecordFailedMenu() {}
void nsRecordFailedMenu::Display(u64 kDown)
{
    printf(CONSOLE_ESC(2J));
    printf(CONSOLE_RED "nsListApplicationRecord failed!, click `+` to exit!\n");
}