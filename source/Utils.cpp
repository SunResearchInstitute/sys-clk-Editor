#include "Utils.h"
#include <sstream>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;
using namespace simpleIniParser;

namespace Utils
{
void Log(string str)
{
    fstream myfile;
    myfile.open("sdmc:/editor-debug.log", fstream::app | fstream::in);
    myfile << str << "\n";
    myfile.close();
}

void ResetConfig()
{
    stringstream ss;
    ss << 0 << hex << uppercase << titles.at(gameSelected).TitleID;
    auto buff = ss.str();
    Ini *config = Ini::parseFile(configFile);
    if (config->findSection(buff, false) != nullptr)
    {
        vector<IniSection *>::iterator it = find(config->sections.begin(), config->sections.end(), config->findSection(buff, false));
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

    if (config->findSection(buff, false) == nullptr)
    {
        config->sections.push_back(new IniSection(IniSectionType::Section, buff));
    }
    if (config->findSection(buff, false)->findFirstOption(ConfigItems.at(configSelected)) == nullptr)
    {
        
        config->findSection(buff, false)->options.push_back(new IniOption(IniOptionType::Option, ConfigItems.at(configSelected), vect.at(selection)));
    }
    else
    {
        config->findSection(buff, false)->findFirstOption(ConfigItems.at(configSelected))->value = vect.at(selection);
    }
    if (config->findSection(titles.at(gameSelected).TitleName, false) == nullptr)
    {
        vector<IniSection *>::iterator it = find(config->sections.begin(), config->sections.end(), config->findSection(buff, false));
        config->sections.insert(it, new IniSection(IniSectionType::SemicolonComment, titles.at(gameSelected).TitleName));
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
    size_t actualAppRecordCnt = 0;
    Result rc;
    rc = nsInitialize();
    if (R_FAILED(rc))
    {
        scene = -1;
        return apps;
    }
    rc = nsListApplicationRecord(appRecords, sizeof(NsApplicationRecord) * 1024, 0, &actualAppRecordCnt);
    if (R_FAILED(rc))
    {
        nsExit();
        scene = -2;
        return apps;
    }
    Utils::Log("Title count: " + to_string(actualAppRecordCnt));
    Title qlaunch;
    qlaunch.TitleID = 0x0100000000001000;
    qlaunch.TitleName = "qlaunch";
    apps.push_back(qlaunch);
    for (u32 i = 0; i < actualAppRecordCnt; i++)
    {
        Utils::Log("tid: " + to_string(appRecords[i].titleID));
        Utils::Log("Title" + getAppName(appRecords[i].titleID));
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
    NacpLanguageEntry *languageEntry = nullptr;
    Result rc;

    memset(&appControlData, 0x00, sizeof(NsApplicationControlData));

    rc = nsGetApplicationControlData(1, Tid, &appControlData, sizeof(NsApplicationControlData), &appControlDataSize);
    if (R_FAILED(rc))
    {
        return "null";
    }
    rc = nacpGetLanguageEntry(&appControlData.nacp, &languageEntry);
    if (R_FAILED(rc))
    {
        return "null";
    }
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
} // namespace Utils
