#include "TitleMenu.h"

void TitleMenu::Update(StateMachine &stateMachine, u64 kDown)
{
    if (kDown & KEY_UP)
        selection--;

    if (kDown & KEY_DOWN)
        selection++;

    if (kDown & KEY_LEFT)
        titlePage--;

    if (kDown & KEY_RIGHT)
        titlePage++;

    //Underflow/Overflow check
    int start = titlePage * max_title_items;
    int end = std::min((int)(Utils::titles.size()), start + max_title_items);
    onScreenItems = end - start;
    if (selection < 0)
        selection = onScreenItems - 1;
    if (onScreenItems - 1 < selection)
        selection = 0;

    if (titlePage < 0)
        titlePage = Utils::maxTitlePages;
    if (Utils::maxTitlePages < titlePage)
        titlePage = 0;

    if (kDown & KEY_A)
    {
        Utils::gameSelected = (titlePage * max_title_items) + selection;
        stateMachine.PushState("config");
        return;
    }

    Utils::printTitles(selection, titlePage, onScreenItems);

    if (kDown & KEY_B)
    {
        stateMachine.PushState("main");
        return;
    }
}