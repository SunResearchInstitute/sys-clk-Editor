#pragma once
#include <switch.h>
#include <vector>
#include <string>
#include "menu.h"

#define center(p, c) (int)((p - c) / 2)

enum class ClkState
{
    NotFound,
    Error,
    Enabled,
    Disabled,
};

namespace Utils
{
extern int gameSelected;
extern int configSelected;
extern std::vector<Title> titles;
extern int maxTitlePages;

void changeConfiguration(const std::vector<std::string> &vect, int selection);
void printConfig(const std::vector<std::string> &configItems, int selection);
std::vector<Title> getAllTitles();
std::string getAppName(u64 Tid);
void printTitles(int selection, int titlePage, int onScreenItems);
void printItems(const std::vector<std::string> &items, std::string menuTitle, int selection);
void getAllConfigItems(std::string option, simpleIniParser::Ini *config, simpleIniParser::IniSection *Section, simpleIniParser::IniOption *Option);
ClkState getClkState();
void resetConfig();
u64 GetControllerInputs();
} // namespace Utils