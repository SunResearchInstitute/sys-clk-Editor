#pragma once

#include <switch.h>
#include <map>
#include <string>
#include <vector>
#include <SimpleIniParser.hpp>

struct Title
{
    u64 TitleID;
    std::string TitleName;
};

const std::vector<std::string> ConfigItems{
    "docked_cpu",
    "docked_gpu",
    "docked_mem",
    "handheld_charging_cpu",
    "handheld_charging_gpu",
    "handheld_charging_mem",
    "handheld_charging_usb_cpu",
    "handheld_charging_usb_gpu",
    "handheld_charging_usb_mem",
    "handheld_charging_official_cpu",
    "handheld_charging_official_gpu",
    "handheld_charging_official_mem",
    "handheld_cpu",
    "handheld_gpu",
    "handheld_mem",
    "Rest to stock"};
const std::vector<std::string> CPUClocks{
    "1785",
    "1683",
    "1581",
    "1428",
    "1326",
    "1224",
    "1122",
    "1020",
    "918",
    "816",
    "714",
    "612",
    "0"};
const std::vector<std::string> MEMClocks{
    "1600",
    "1331",
    "1065",
    "800",
    "665",
    "0"};
const std::vector<std::string> GPUClocks{
    "921",
    "844",
    "768",
    "691",
    "614",
    "537",
    "460",
    "384",
    "307",
    "230",
    "153",
    "76",
    "0"};
const int max_title_items = 43;
const std::string logFlag = "sdmc:/config/sys-clk/log.flag";
const std::string configFile = "sdmc:/config/sys-clk/config.ini";
const std::string boot2Flag = "sdmc:/atmosphere/titles/00FF0000636C6BFF/flags/boot2.flag";
const u64 sysClkTid = 0x00FF0000636C6BFF;

void ChangeConfiguration(const std::vector<std::string> &vect);
void printConfig(const std::vector<std::string> &configItems);
void menuMainLoop();
std::vector<Title> *getAllTitles();
std::string getAppName(u64 Tid);
void printTitles();
void printItems(const std::vector<std::string> &items, std::string menuTitle);
void getAllConfigItems(std::string option, simpleIniParser::Ini *config, simpleIniParser::IniSection *Section, simpleIniParser::IniOption *Option);
bool IsClkActive();
void ResetConfig();
