#include "Utils.h"
#include <sstream>
#include <algorithm>
#include <cstring>

using namespace std;
using namespace simpleIniParser;

namespace Utils
{
void startErrorScreen(Result rc)
{
    char str[35];
    sprintf(str, "Error: 0x%x", rc);
    printf(CONSOLE_RED "\x1b[21;%d%s", center(80, (int)strlen(str)), str);
    printf(CONSOLE_RED "\x1b[22;%dPress `+` to exit!", center(80, 17));
    consoleUpdate(nullptr);
    while (appletMainLoop())
    {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        if (kDown & KEY_PLUS)
        {
            consoleExit(nullptr);
            scene = -69;
        }
    }
}

void resetConfig()
{
    stringstream ss;
    ss << 0 << hex << uppercase << titles.at(gameSelected).TitleID;
    auto buff = ss.str();
    Ini *config = Ini::parseFile(CONFIG);
    if (config->findSection(buff, false) != nullptr)
    {
        vector<IniSection *>::iterator it = find(config->sections.begin(), config->sections.end(), config->findSection(buff, false));
        config->sections.erase(it);
        config->writeToFile(CONFIG);
    }
    delete config;
}

void changeConfiguration(const vector<string> &vect)
{
    stringstream ss;
    ss << 0 << hex << uppercase << titles.at(gameSelected).TitleID;
    auto buff = ss.str();
    Ini *config = Ini::parseFile(CONFIG);

    IniSection *section = config->findSection(buff, false);
    if (section == nullptr)
    {
        config->sections.push_back(new IniSection(IniSectionType::Section, buff));
        section = config->findSection(buff, false);
    }

    if (section->findFirstOption(ConfigItems.at(configSelected)) == nullptr)
        section->options.push_back(new IniOption(IniOptionType::Option, ConfigItems.at(configSelected), vect.at(selection)));

    else
        section->findFirstOption(ConfigItems.at(configSelected))->value = vect.at(selection);

    if (section->findFirstOption(titles.at(gameSelected).TitleName, false, IniOptionType::SemicolonComment, IniOptionSearchField::Value) == nullptr)
        section->options.insert(section->options.begin(), new IniOption(IniOptionType::SemicolonComment, "", titles.at(gameSelected).TitleName));

    config->writeToFile(CONFIG);
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
    printf(CONSOLE_MAGENTA "\x1b[0;%dH%s\n", center(80, menuTitle.size()), menuTitle.c_str());
    for (int i = 0; i < (int)items.size(); i++)
    {
        const char *prefix = " ";
        if (selection == i)
            prefix = ">";
        printf(CONSOLE_WHITE "%s%s", prefix, items[i].c_str());
        if (menuTitle == "GPU Clocks")
        {
            printf(" %s", GPUCmt[i].c_str());
        }
        else if (menuTitle == "CPU Clocks")
        {
            printf(" %s", CPUCmt[i].c_str());
        }
        else if (menuTitle == "MEM Clocks")
        {
            printf(" %s", MEMCmt[i].c_str());
        }
        printf("\n");
    }
}

void printConfig(const vector<string> &configItems)
{
    Title title = titles.at(gameSelected);
    stringstream ss;
    ss << 0 << hex << uppercase << title.TitleID << ": " << title.TitleName;
    string buff = ss.str();
    printf(CONSOLE_MAGENTA "\x1b[0;%dH%s\n", center(80, buff.size()), buff.c_str());
    Ini *config = Ini::parseFile(CONFIG);
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
        IniSection *section = config->findSection(buff, false);
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
    s32 actualAppRecordCnt = 0;
    Result rc;
    rc = nsListApplicationRecord(appRecords, 1024, 0, &actualAppRecordCnt);
    if (R_FAILED(rc))
    {
        Utils::startErrorScreen(rc);
        return apps;
    }

    Title qlaunch;
    qlaunch.TitleID = 0x0100000000001000;
    qlaunch.TitleName = "qlaunch";
    apps.push_back(qlaunch);
    for (s32 i = 0; i < actualAppRecordCnt; i++)
    {
        Title title;
        title.TitleID = appRecords[i].application_id;
        title.TitleName = getAppName(appRecords[i].application_id);
        apps.push_back(title);
    }
    delete[] appRecords;
    return apps;
}

string getAppName(u64 Tid)
{
    NsApplicationControlData appControlData;
    size_t appControlDataSize = 0;
    NacpLanguageEntry *languageEntry = nullptr;
    Result rc;

    memset(&appControlData, 0x00, sizeof(NsApplicationControlData));
    
    rc = nsGetApplicationControlData(NsApplicationControlSource::NsApplicationControlSource_Storage, Tid, &appControlData, sizeof(NsApplicationControlData), &appControlDataSize);
    if (R_FAILED(rc))
    {
        stringstream ss;
        ss << 0 << hex << uppercase << Tid;
        return ss.str();
    }
    rc = nacpGetLanguageEntry(&appControlData.nacp, &languageEntry);
    if (R_FAILED(rc))
    {
        stringstream ss;
        ss << 0 << hex << uppercase << Tid;
        return ss.str();
    }
    return string(languageEntry->name);
}

bool isClkActive()
{
    Result rc;
    u64 pid = 0;
    rc = pmdmntGetProcessId(&pid, sysClkTid);
    if (pid < 0 || R_FAILED(rc))
        return false;

    return true;
}
} // namespace Utils
