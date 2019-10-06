#include "Scenes/scenes.h"
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
std::vector<Title> titles;
int maxTitlePages = titles.size() / max_title_items;
std::vector<std::string> firstMenuItems{"Games", "PlaceHldr", "PlaceHldr", "Delete config"};

void menuMainLoop()
{
    Scene *currentScene;
    int lastScene = scene;
    if (scene == 0)
    {
        currentScene = new MainMenu();

        if (!filesystem::exists(logFlag))
            firstMenuItems[1] = "Toggle sys-clk Logging: Disabled";
        else
            firstMenuItems[1] = "Toggle sys-clk Logging: Enabled";

        if (Utils::isClkActive())
            firstMenuItems[2] = "sys-clk is enabled!";
        else
            firstMenuItems[2] = "sys-clk is disabled!";

        Utils::printItems(firstMenuItems, "Main Menu");
        titles = Utils::getAllTitles();
    }
    else
        return;

    while (appletMainLoop())
    {
        if (lastScene != scene)
        {
            switch (scene)
            {
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
                return;
            }
        }

        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        if (kDown & KEY_PLUS || (scene == 0 && kDown & KEY_B))
        {
            consoleExit(nullptr);
            delete currentScene;
            return;
        }
        currentScene->Display(kDown);
        if (lastScene != scene)
        {
            lastScene = scene;
            delete currentScene;
        }
        consoleUpdate(nullptr);
    }
}
