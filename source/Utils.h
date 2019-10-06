#pragma once
#include <switch.h>
#include <vector>
#include <string>
#include "menu.h"

#define center(p, c) (int)((p - c) / 2)

namespace Utils
{
void startErrorScreen(Result rc);
void changeConfiguration(const std::vector<std::string> &vect);
void printConfig(const std::vector<std::string> &configItems);
std::vector<Title> getAllTitles();
std::string getAppName(u64 Tid);
void printTitles();
void printItems(const std::vector<std::string> &items, std::string menuTitle);
void getAllConfigItems(std::string option, simpleIniParser::Ini *config, simpleIniParser::IniSection *Section, simpleIniParser::IniOption *Option);
bool isClkActive();
void resetConfig();
} // namespace Utils