#include "Utils.h"
#include <sstream>
#include <algorithm>
#include <cstring>

using namespace std;
using namespace simpleIniParser;

namespace Utils
{
void printError(string str)
{
    scene = -1;
    printf(CONSOLE_RED "\x1b[22;%dH%s", center(80, str.size()), str.c_str());
}

void resetConfig()
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

void changeConfiguration(const vector<string> &vect)
{
    stringstream ss;
    ss << 0 << hex << uppercase << titles.at(gameSelected).TitleID;
    auto buff = ss.str();
    Ini *config = Ini::parseFile(configFile);

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
    int actualAppRecordCnt = 0;
    Result rc;

    rc = nsListApplicationRecord(appRecords, sizeof(NsApplicationRecord) * 1024, 0, &actualAppRecordCnt);
    if (R_FAILED(rc))
    {
        Utils::printError("Failed to get Applications!");
        return apps;
    }

    Title systemDefault;
    systemDefault.TitleID = 0x0999999999999999;
    systemDefault.TitleName = "System-wide Default";
    apps.push_back(systemDefault);
    Title qlaunch;
    qlaunch.TitleID = 0x0100000000001000;
    qlaunch.TitleName = "qlaunch";
    apps.push_back(qlaunch);
    for (int i = 0; i < actualAppRecordCnt; i++)
    {
        Title title;
        title.TitleID = appRecords[i].titleID;
        title.TitleName = getAppName(appRecords[i].titleID);
        apps.push_back(title);
    }
    delete appRecords;
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
    rc = pmdmntGetTitlePid(&pid, sysClkTid);
    if (pid < 0 || R_FAILED(rc))
        return false;

    return true;
}

bool areTempsEnabled()
{
    Ini *config = Ini::parseFile(configFile);
    IniSection *section = config->findSection("value", false);
    if (section != nullptr)
    {
        IniOption *option1 = section->findFirstOption("csv_write_interval_ms", false);
        IniOption *option2 = section->findFirstOption("temp_log_interval_ms", false);
        if (option1 != nullptr && option2 != nullptr)
            if (option1->value != "0" && option2->value != "0")
                return true;
    }
    return false;
}
} // namespace Utils
