//Shit code i know :shrek:
#include "menu.h"
#include "Scenes/scenes.h"
#include <algorithm>
#include <cstring>
#include <filesystem>
#include "Utils.h"

using namespace std;
using namespace simpleIniParser;

signed int selection = 0;
int gameSelected;
int configSelected = 0;
int scene = 0;
int title_page = 0;
int onscreen_items;
std::vector<Title> titles = Utils::getAllTitles();
int maxTitlePages = titles.size() / max_title_items;
std::vector<std::string> firstMenuItems{"Games", "PlaceHldr", "PlaceHldr", "Delete Config"};

void menuMainLoop()
{
    Scene *currentScene;
    if (!filesystem::exists(logFlag))
        firstMenuItems[1] = ("Toggle sys-clk Logging: Disabled");
    else
        firstMenuItems[1] = ("Toggle sys-clk Logging: Enabled");

    if (Utils::IsClkActive())
        firstMenuItems[2] = "sys-clk is enabled!";
    else
        firstMenuItems[2] = "sys-clk is disabled!";

    Utils::printItems(firstMenuItems, "Main Menu");
    Utils::Log("menu has been written!");
    while (appletMainLoop())
    {
        switch (scene)
        {
        case -69:
            Utils::Log("Case -69");
            return;
        case -1:
            currentScene = new nsFailedMenu();
            break;
        case -2:
            currentScene = new nsRecordFailedMenu();
            break;
        case 0:
            currentScene = new MainMenu();
            break;
        case 1:
            currentScene = new TitleMenu();
            break;
        case 2:
            currentScene = new ConfigMenu();
            break;
        case 3:
            currentScene = new CPUMenu();
            break;
        case 4:
            currentScene = new GPUMenu();
            break;
        case 5:
            currentScene = new MEMMenu();
            break;
        default:
            Utils::Log("invalid case! " + to_string(scene));
            return;
        }
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        if (kDown & KEY_PLUS)
        {
            delete currentScene;
            return;
        }
        currentScene->Display(kDown);
        delete currentScene;
        consoleUpdate(nullptr);
    }
}
