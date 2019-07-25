//Shit code i know :shrek:
#include "menu.h"
#include <algorithm>
#include <cstring>
#include <SimpleIniParser.hpp>
#include <filesystem>

using namespace std;
using namespace simpleIniParser;

static signed int s_selection = 0;
static int s_gameselected;
static int s_configselected = 0;
static int s_scene = 0;
static int s_title_page = 0;
static int s_onscreen_items;

static vector<Title> *s_titles = getAllTitles();
static int s_max_title_pages = s_titles->size() / max_title_items;
static vector<string> FirstMenuItems{"Games", "PlaceHldr", "PlaceHldr", "Delete Config"};

void menuMainLoop()
{
    if (s_scene == 0)
    {
        if (!filesystem::exists(logFlag))
            FirstMenuItems[1] = ("Toggle sys-clk Logging: Disabled");
        else
            FirstMenuItems[1] = ("Toggle sys-clk Logging: Enabled");

        if (IsClkActive())
            FirstMenuItems[2] = "sys-clk is enabled!";
        else
            FirstMenuItems[2] = "sys-clk is disabled!";

        printItems(FirstMenuItems, "Main Menu");
    }

    while (appletMainLoop())
    {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        bool needsRefresh = false;

        if (s_scene == 0)
        {
            if (kDown & KEY_UP)
            {
                s_selection--;
                needsRefresh = true;
            }
            if (kDown & KEY_DOWN)
            {
                s_selection++;
                needsRefresh = true;
            }

            // check for under/overflow
            long int size = FirstMenuItems.size();
            if (s_selection < 0)
                s_selection = size - 1;
            if (size <= s_selection)
                s_selection = 0;

            if (kDown & KEY_A)
            {
                switch (s_selection)
                {
                case 0:
                    s_scene = 1;
                    s_selection = 0;
                    printf(CONSOLE_ESC(2J));
                    printTitles();
                    break;
                case 1:
                {
                    if (!filesystem::exists(logFlag))
                    {
                        fclose(fopen(logFlag.c_str(), "w"));
                        FirstMenuItems[1] = "Toggle sys-clk Logging: Enabled";
                    }
                    else
                    {
                        remove(logFlag.c_str());
                        FirstMenuItems[1] = "Toggle sys-clk Logging: Disabled";
                    }
                    needsRefresh = true;
                    break;
                }
                case 3:
                    fclose(fopen(configFile.c_str(), "w"));
                    s_scene = 1;
                    s_selection = 0;
                    printf(CONSOLE_ESC(2J));
                    printTitles();
                    break;
                case 2:
                {
                    pmshellInitialize();
                    if (IsClkActive())
                    {
                        if (R_SUCCEEDED(pmshellTerminateProcessByTitleId(sysClkTid)))
                        {
                            FirstMenuItems[2] = "sys-clk is disabled!";
                            remove(boot2Flag.c_str());
                        }
                    }
                    else
                    {
                        u64 pid;
                        if (R_SUCCEEDED(pmshellLaunchProcess(0, sysClkTid, FsStorageId_None, &pid)))
                        {
                            FirstMenuItems[2] = "sys-clk is enabled!";
                            fopen(boot2Flag.c_str(), "w");
                        }
                    }
                    pmshellExit();
                    needsRefresh = true;
                    break;
                }
                }
            }

            if (needsRefresh)
            {
                printf(CONSOLE_ESC(2J));
                printItems(FirstMenuItems, "Main Menu");
            }

            if (kDown & KEY_B)
            {
                delete s_titles;
                return;
            }
        }
        else if (s_scene == 1)
        {
            if (kDown & KEY_UP)
            {
                s_selection--;
                needsRefresh = true;
            }
            if (kDown & KEY_DOWN)
            {
                s_selection++;
                needsRefresh = true;
            }

            if (kDown & KEY_LEFT)
            {
                s_title_page--;
                needsRefresh = true;
            }
            if (kDown & KEY_RIGHT)
            {
                s_title_page++;
                needsRefresh = true;
            }

            //Underflow/Overflow check
            int start = s_title_page * max_title_items;
            int end = std::min(static_cast<int>(s_titles->size()), start + max_title_items);
            s_onscreen_items = end - start;
            if (s_selection < 0)
                s_selection = s_onscreen_items - 1;
            if (s_onscreen_items - 1 < s_selection)
                s_selection = 0;

            if (s_title_page < 0)
                s_title_page = s_max_title_pages;
            if (s_max_title_pages < s_title_page)
                s_title_page = 0;

            if (kDown & KEY_A)
            {
                s_scene = 2;
                s_gameselected = (s_title_page * max_title_items) + s_selection;
                s_selection = 0;
                printf(CONSOLE_ESC(2J));
                printConfig(ConfigItems);
            }

            if (needsRefresh)
            {
                printf(CONSOLE_ESC(2J));
                printTitles();
            }

            if (kDown & KEY_B)
            {
                printf(CONSOLE_ESC(2J));
                s_scene = 0;
                s_selection = 0;
                printItems(FirstMenuItems, "Main Menu");
            }
        }
        else if (s_scene == 2)
        {
            if (kDown & KEY_UP)
            {
                s_selection--;
                needsRefresh = true;
            }

            if (kDown & KEY_DOWN)
            {
                s_selection++;
                needsRefresh = true;
            }

            // check for under/overflow
            long int size = ConfigItems.size();
            if (s_selection < 0)
                s_selection = size - 1;
            if (size <= s_selection)
                s_selection = 0;

            if (kDown & KEY_A)
            {
                printf(CONSOLE_ESC(2J));
                s_configselected = s_selection;
                switch (s_selection)
                {
                //CPU
                case 0:
                case 3:
                case 6:
                case 9:
                case 12:
                    s_selection = 0;
                    s_scene = 3;
                    printItems(CPUClocks, "CPU Clocks");
                    break;
                //GPU
                case 1:
                case 4:
                case 7:
                case 10:
                case 13:
                    s_selection = 0;
                    s_scene = 4;
                    printItems(GPUClocks, "GPU Clocks");
                    break;
                //RAM
                case 2:
                case 5:
                case 8:
                case 11:
                case 14:
                    s_selection = 0;
                    s_scene = 5;
                    printItems(MEMClocks, "RAM Clocks");
                    break;
                case 15:
                    ResetConfig();
                    s_scene = 1;
                    s_selection = s_gameselected - (s_title_page * max_title_items);
                    printTitles();
                    break;
                default:
                    break;
                }
            }

            if (needsRefresh)
            {
                printf(CONSOLE_ESC(2J));
                printConfig(ConfigItems);
            }

            if (kDown & KEY_B)
            {
                printf(CONSOLE_ESC(2J));
                s_scene = 1;
                s_selection = s_gameselected - (s_title_page * max_title_items);
                printTitles();
            }
        }
        //CPU
        else if (s_scene == 3)
        {
            if (kDown & KEY_UP)
            {
                s_selection--;
                needsRefresh = true;
            }

            if (kDown & KEY_DOWN)
            {
                s_selection++;
                needsRefresh = true;
            }

            long int size = CPUClocks.size();
            if (s_selection < 0)
                s_selection = size - 1;
            if (size <= s_selection)
                s_selection = 0;

            if (kDown & KEY_A)
            {
                ChangeConfiguration(CPUClocks);
                s_scene = 2;
                s_selection = s_configselected;
                printf(CONSOLE_ESC(2J));
                printConfig(ConfigItems);
            }

            if (needsRefresh)
            {
                printf(CONSOLE_ESC(2J));
                printItems(CPUClocks, "CPU Clocks");
            }

            if (kDown & KEY_B)
            {
                printf(CONSOLE_ESC(2J));
                s_scene = 2;
                s_selection = s_configselected;
                printConfig(ConfigItems);
            }
        }
        //GPU
        else if (s_scene == 4)
        {
            if (kDown & KEY_UP)
            {
                s_selection--;
                needsRefresh = true;
            }

            if (kDown & KEY_DOWN)
            {
                s_selection++;
                needsRefresh = true;
            }

            long int size = GPUClocks.size();
            if (s_selection < 0)
                s_selection = size - 1;
            if (size <= s_selection)
                s_selection = 0;

            if (kDown & KEY_A)
            {
                ChangeConfiguration(GPUClocks);
                s_scene = 2;
                s_selection = s_configselected;
                printf(CONSOLE_ESC(2J));
                printConfig(ConfigItems);
            }

            if (needsRefresh)
            {
                printf(CONSOLE_ESC(2J));
                printItems(GPUClocks, "GPU Clocks");
            }

            if (kDown & KEY_B)
            {
                printf(CONSOLE_ESC(2J));
                s_scene = 2;
                s_selection = s_configselected;
                printConfig(ConfigItems);
            }
        }
        //MEM
        else if (s_scene == 5)
        {
            if (kDown & KEY_UP)
            {
                s_selection--;
                needsRefresh = true;
            }

            if (kDown & KEY_DOWN)
            {
                s_selection++;
                needsRefresh = true;
            }

            long int size = MEMClocks.size();
            if (s_selection < 0)
                s_selection = size - 1;
            if (size <= s_selection)
                s_selection = 0;

            if (kDown & KEY_A)
            {
                ChangeConfiguration(MEMClocks);
                s_scene = 2;
                s_selection = s_configselected;
                printf(CONSOLE_ESC(2J));
                printConfig(ConfigItems);
            }

            if (needsRefresh)
            {
                printf(CONSOLE_ESC(2J));
                printItems(MEMClocks, "RAM Clocks");
            }

            if (kDown & KEY_B)
            {
                printf(CONSOLE_ESC(2J));
                s_scene = 2;
                s_selection = s_configselected;
                printConfig(ConfigItems);
            }
        }
        else if (s_scene == -1)
        {
            printf(CONSOLE_ESC(2J));
            printf(CONSOLE_RED "nsInitialize Failed!, click `+` to exit!\n");
        }
        else if (s_scene - 2)
        {
            printf(CONSOLE_ESC(2J));
            printf(CONSOLE_RED "nsListApplicationRecord failed!, click `+` to exit!\n");
        }

        if (kDown & KEY_PLUS)
        {
            delete s_titles;
            return;
        }

        consoleUpdate(nullptr);
    }
}

void ResetConfig()
{
    stringstream ss;
    ss << 0 << hex << uppercase << s_titles->at(s_gameselected).TitleID;
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
    ss << 0 << hex << uppercase << s_titles->at(s_gameselected).TitleID;
    auto buff = ss.str();
    Ini *config = Ini::parseFile(configFile);

    if (config->findSection(buff) == nullptr)
    {
        config->sections.push_back(new IniSection(SECTION, buff));
    }
    if (config->findSection(buff)->findFirstOption(ConfigItems.at(s_configselected)) == nullptr)
    {
        config->findSection(buff)->options.push_back(new IniOption(ConfigItems.at(s_configselected), vect.at(s_selection)));
    }
    else
    {
        config->findSection(buff)->findFirstOption(ConfigItems.at(s_configselected))->value = vect.at(s_selection);
    }
    if (config->findSection(s_titles->at(s_gameselected).TitleName) == nullptr)
    {
        vector<IniSection *>::iterator it = find(config->sections.begin(), config->sections.end(), config->findSection(buff));
        config->sections.insert(it, new IniSection(SEMICOLON_COMMENT, s_titles->at(s_gameselected).TitleName));
    }
    config->writeToFile(configFile);
    delete config;
}

//Thanks WerWolv :)
void printTitles()
{
    printf(CONSOLE_MAGENTA "\x1b[0;36HGame List\n");
    int start = s_title_page * max_title_items;
    int end = std::min(static_cast<int>(s_titles->size()), start + max_title_items);
    int j = 0;
    for (int i = start; i < end; i++)
    {
        const char *prefix = " ";
        if (s_selection == j)
            prefix = ">";
        printf(CONSOLE_WHITE "%s%s\n", prefix, s_titles->at(i).TitleName.c_str());
        j++;
    }
    s_onscreen_items = j;
    printf(CONSOLE_MAGENTA "Page %d/%d", s_title_page + 1, s_max_title_pages + 1);
}

void printItems(const vector<string> &items, string menuTitle)
{
    printf(CONSOLE_MAGENTA "\x1b[0;%dH%s\n", (40 - ((int)menuTitle.size() / 2)), menuTitle.c_str());
    for (int i = 0; i < (int)items.size(); i++)
    {
        const char *prefix = " ";
        if (s_selection == i)
            prefix = ">";
        printf(CONSOLE_WHITE "%s%s\n", prefix, items[i].c_str());
    }
}

void printConfig(const vector<string> &configItems)
{
    Title title = s_titles->at(s_gameselected);
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
        if (s_selection == i)
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

vector<Title> *getAllTitles()
{
    vector<Title> *apps = new vector<Title>;
    NsApplicationRecord *appRecords = new NsApplicationRecord[1024]; // Nobody's going to have more than 1024 games hopefully...
    size_t actualAppRecordCnt = 0;
    Result rc;
    rc = nsInitialize();
    if (R_FAILED(rc))
    {
        s_scene = -1;
        return apps;
    }
    rc = nsListApplicationRecord(appRecords, sizeof(NsApplicationRecord) * 1024, 0, &actualAppRecordCnt);
    if (R_FAILED(rc))
    {
        nsExit();
        s_scene = -2;
        return apps;
    }
    Title qlaunch;
    qlaunch.TitleID = 0x0100000000001000;
    qlaunch.TitleName = "qlaunch";
    apps->push_back(qlaunch);
    for (u32 i = 0; i < actualAppRecordCnt; i++)
    {
        Title title;
        title.TitleID = appRecords[i].titleID;
        title.TitleName = getAppName(appRecords[i].titleID);
        apps->push_back(title);
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
