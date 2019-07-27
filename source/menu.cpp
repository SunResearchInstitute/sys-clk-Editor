//Shit code i know :shrek:
#include "menu.h"
#include "Scenes/scenes.h"
#include <algorithm>
#include <cstring>
#include <SimpleIniParser.hpp>
#include <filesystem>

using namespace std;
using namespace simpleIniParser;

signed int selection = 0;
int gameSelected;
int configSelected = 0;
int scene = 0;
int title_page = 0;
int onscreen_items;
std::vector<Title> titles = getAllTitles();
int maxTitlePages = titles.size() / max_title_items;
std::vector<std::string> firstMenuItems{"Games", "PlaceHldr", "PlaceHldr", "Delete Config"};

void menuMainLoop()
{
    Scene *currentScene;
    currentScene = new MainMenu();
    if (scene == 0)
    {
        if (!filesystem::exists(logFlag))
            firstMenuItems[1] = ("Toggle sys-clk Logging: Disabled");
        else
            firstMenuItems[1] = ("Toggle sys-clk Logging: Enabled");

        if (IsClkActive())
            firstMenuItems[2] = "sys-clk is enabled!";
        else
            firstMenuItems[2] = "sys-clk is disabled!";

        printItems(firstMenuItems, "Main Menu");
    }

    while (appletMainLoop())
    {
        delete currentScene;
        switch (scene)
        {
        case -69:
            delete currentScene;
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
            currentScene = new RAMMenu();
        default:
            break;
        }
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        if (kDown & KEY_PLUS)
        {
            delete currentScene;
            return;
        }
        currentScene->Display(kDown);
        consoleUpdate(nullptr);
    }
}

void ResetConfig()
{
    stringstream ss;
    ss << 0 << hex << uppercase << titles.at(gameSelected).TitleID;
    auto buff = ss.str();
    Ini *config = Ini::parseFile(configFile);
    if (config->findSection(buff) != nullptr)
    {
        vector<IniSection *>::iterator it = find(config->sections.begin(), config->sections.end(), config->findSection(buff));
        config->sections.erase(it);
        config->writeToFile(configFile);
    }
    delete config;
}

void ChangeConfiguration(const vector<string> &vect)
{
    stringstream ss;
    ss << 0 << hex << uppercase << titles.at(gameSelected).TitleID;
    auto buff = ss.str();
    Ini *config = Ini::parseFile(configFile);

    if (config->findSection(buff) == nullptr)
    {
        config->sections.push_back(new IniSection(SECTION, buff));
    }
    if (config->findSection(buff)->findFirstOption(ConfigItems.at(configSelected)) == nullptr)
    {
        config->findSection(buff)->options.push_back(new IniOption(ConfigItems.at(configSelected), vect.at(selection)));
    }
    else
    {
        config->findSection(buff)->findFirstOption(ConfigItems.at(configSelected))->value = vect.at(selection);
    }
    if (config->findSection(titles.at(gameSelected).TitleName) == nullptr)
    {
        vector<IniSection *>::iterator it = find(config->sections.begin(), config->sections.end(), config->findSection(buff));
        config->sections.insert(it, new IniSection(SEMICOLON_COMMENT, titles.at(gameSelected).TitleName));
    }
    config->writeToFile(configFile);
    delete config;
}

//Thanks WerWolv :)
void printTitles()
{
    printf(CONSOLE_MAGENTA "\x1b[0;36HGame List\n");
    int start = title_page * max_title_items;
    int end = std::min(static_cast<int>(titles.size()), start + max_title_items);
    int j = 0;
    for (int i = start; i < end; i++)
    {
        const char *prefix = " ";
        if (selection == j)
            prefix = ">";
        printf(CONSOLE_WHITE "%s%s\n", prefix, titles.at(i).TitleName.c_str());
        j++;
    }
    onscreen_items = j;
    printf(CONSOLE_MAGENTA "Page %d/%d", title_page + 1, maxTitlePages + 1);
}

void printItems(const vector<string> &items, string menuTitle)
{
    printf(CONSOLE_MAGENTA "\x1b[0;%dH%s\n", (40 - ((int)menuTitle.size() / 2)), menuTitle.c_str());
    for (int i = 0; i < (int)items.size(); i++)
    {
        const char *prefix = " ";
        if (selection == i)
            prefix = ">";
        printf(CONSOLE_WHITE "%s%s\n", prefix, items[i].c_str());
    }
}

void printConfig(const vector<string> &configItems)
{
    Title title = titles.at(gameSelected);
    stringstream ss;
    ss << 0 << hex << uppercase << title.TitleID << ": " << title.TitleName;
    string buff = ss.str();
    printf(CONSOLE_MAGENTA "\x1b[0;%dH%s\n", (40 - ((int)buff.size() / 2)), buff.c_str());
    Ini *config = Ini::parseFile(configFile);
    stringstream ss2;
    ss2 << 0 << hex << uppercase << title.TitleID;
    buff = ss2.str();
    for (int i = 0; i < (int)ConfigItems.size(); i++)
    {
        const char *prefix = " ";
        if (selection == i)
            prefix = ">";
        if (i == (int)configItems.size() - 1)
        {
            printf(CONSOLE_WHITE "%s%s", prefix, configItems[i].c_str());
            break;
        }
        IniSection *section = config->findSection(buff);
        if (section != nullptr)
        {
            IniOption *option = section->findFirstOption(configItems[i].c_str());
            if (option != nullptr)
                printf(CONSOLE_WHITE "%s%s: %s\n", prefix, configItems[i].c_str(), option->value.c_str());
            else
                printf(CONSOLE_WHITE "%s%s: 0\n", prefix, configItems[i].c_str());
        }
        else
            printf(CONSOLE_WHITE "%s%s: 0\n", prefix, configItems[i].c_str());
    }
    delete config;
}

vector<Title> getAllTitles()
{
    vector<Title> apps;
    NsApplicationRecord *appRecords = new NsApplicationRecord[1024]; // Nobody's going to have more than 1024 games hopefully...
    size_t actualAppRecordCnt = 0;
    Result rc;
    rc = nsInitialize();
    if (R_FAILED(rc))
    {
        scene = -1;
        return apps;
    }
    rc = nsListApplicationRecord(appRecords, sizeof(NsApplicationRecord) * 1024, 0, &actualAppRecordCnt);
    if (!R_FAILED(rc))
    {
        nsExit();
        scene = -2;
        return apps;
    }
    Title qlaunch;
    qlaunch.TitleID = 0x0100000000001000;
    qlaunch.TitleName = "qlaunch";
    apps.push_back(qlaunch);
    for (u32 i = 0; i < actualAppRecordCnt; i++)
    {
        Title title;
        title.TitleID = appRecords[i].titleID;
        title.TitleName = getAppName(appRecords[i].titleID);
        apps.push_back(title);
    }

    nsExit();
    return apps;
}

string getAppName(u64 Tid)
{
    NsApplicationControlData appControlData;
    size_t appControlDataSize = 0;
    NsApplicationContentMetaStatus appContentMetaStatus;
    NacpLanguageEntry *languageEntry = nullptr;

    memset(&appControlData, 0x00, sizeof(NsApplicationControlData));

    nsGetApplicationControlData(1, Tid, &appControlData, sizeof(NsApplicationControlData), &appControlDataSize);
    nsListApplicationContentMetaStatus(Tid, 0, &appContentMetaStatus, sizeof(NsApplicationContentMetaStatus), nullptr);
    nacpGetLanguageEntry(&appControlData.nacp, &languageEntry);

    return string(languageEntry->name);
}

bool IsClkActive()
{
    pmdmntInitialize();
    u64 pid = 0;
    pmdmntGetTitlePid(&pid, 0x00FF0000636C6BFF);
    if (pid > 0)
    {
        pmdmntExit();
        return true;
    }
    pmdmntExit();
    return false;
}
