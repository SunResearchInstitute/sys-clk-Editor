#include "TitleMenu.h"

TitleMenu::TitleMenu() {}
TitleMenu::~TitleMenu() {}
void TitleMenu::Display(u64 kDown)
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

    if (kDown & KEY_LEFT)
    {
        title_page--;
        needsRefresh = true;
    }
    if (kDown & KEY_RIGHT)
    {
        title_page++;
        needsRefresh = true;
    }

    //Underflow/Overflow check
    int start = title_page * max_title_items;
    int end = std::min(static_cast<int>(titles.size()), start + max_title_items);
    onscreen_items = end - start;
    if (selection < 0)
        selection = onscreen_items - 1;
    if (onscreen_items - 1 < selection)
        selection = 0;

    if (title_page < 0)
        title_page = maxTitlePages;
    if (maxTitlePages < title_page)
        title_page = 0;

    if (kDown & KEY_A)
    {
        scene = 2;
        gameSelected = (title_page * max_title_items) + selection;
        selection = 0;
        printf(CONSOLE_ESC(2J));
        Utils::printConfig(ConfigItems);
    }

    if (needsRefresh)
    {
        printf(CONSOLE_ESC(2J));
        Utils::printTitles();
    }

    if (kDown & KEY_B)
    {
        printf(CONSOLE_ESC(2J));
        scene = 0;
        selection = 0;
        Utils::printItems(firstMenuItems, "Main Menu");
    }
}