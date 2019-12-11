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
    "Reset to stock"};
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
const std::vector<std::string> CPUCmt{
    "",
    "",
    "",
    "",
    "",
    "-> sdev OC",
    "",
    "-> official docked and handheld",
    "",
    "",
    "",
    "",
    "-> use default clock"};
const std::vector<std::string> MEMClocks{
    "1600",
    "1331",
    "1065",
    "800",
    "665",
    "0"};
const std::vector<std::string> MEMCmt{
    "-> official docked",
    "-> official handheld",
    "",
    "",
    "",
    "-> use default clock"};
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
const std::vector<std::string> GPUCmt{
    "",
    "",
    "-> official docked/max handheld with USB charging",
    "",
    "",
    "",
    "-> max handheld w/o charging",
    //sys-clk doc states two offical clocks?
    "-> official handheld",
    "-> official handheld",
    "",
    "",
    "",
    "-> use default clock"};
const int max_title_items = 43;
#define LOG "sdmc:/config/sys-clk/log.flag"
#define CONFIG "sdmc:/config/sys-clk/config.ini"
#define FLAGFOLDER "sdmc:/atmosphere/contents/00FF0000636C6BFF/flags/"
#define BOOT2FLAG FLAGFOLDER "boot2.flag"
const u64 sysClkTid = 0x00FF0000636C6BFF;

extern signed int selection;
extern int gameSelected;
extern int configSelected;
extern int scene;
extern int title_page;
extern int onscreen_items;
extern std::vector<Title> titles;
extern int maxTitlePages;
extern std::vector<std::string> firstMenuItems;

void menuMainLoop();