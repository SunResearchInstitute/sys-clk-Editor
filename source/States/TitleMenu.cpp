#include "TitleMenu.h"

void TitleMenu::Update(StateMachine &stateMachine, u64 kDown)
{
    if (kDown & KEY_UP)
        selection--;

    if (kDown & KEY_DOWN)
        selection++;

    if (kDown & KEY_LEFT)
        Utils::title_page--;

    if (kDown & KEY_RIGHT)
        Utils::title_page++;

    //Underflow/Overflow check
    int start = Utils::title_page * max_title_items;
    int end = std::min((int)(Utils::titles.size()), start + max_title_items);
    Utils::onscreen_items = end - start;
    if (selection < 0)
        selection = Utils::onscreen_items - 1;
    if (Utils::onscreen_items - 1 < selection)
        selection = 0;

    if (Utils::title_page < 0)
        Utils::title_page = Utils::maxTitlePages;
    if (Utils::maxTitlePages < Utils::title_page)
        Utils::title_page = 0;

    if (kDown & KEY_A)
    {
        Utils::gameSelected = (Utils::title_page * max_title_items) + selection;
        stateMachine.PushState("config");
        return;
    }

    Utils::printTitles(selection);

    if (kDown & KEY_B)
    {
        stateMachine.PushState("main");
        return;
    }
}