#include "DataManager.h"

#include <string>

DataManager::DataManager()
{
}

std::string DataManager::GetDataDate()
{
    time_t now = time(NULL);
    struct tm time_info;
    localtime_s(&time_info, &now);
    
    if (time_info.tm_hour > 1)
        time_info.tm_mday -= 1;
    else time_info.tm_mday -= 2;
    
    mktime(&time_info);

    int year = time_info.tm_year + 1900;
    int month = time_info.tm_mon + 1;
    int day = time_info.tm_mday;

    std::string date = std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);
    return date;
}

std::string DataManager::FormatUnit(long val)
{
    if (val >= 1000000000000)
    {
        int unit = val / 1000000000000;
        int remain = val % 1000000000000;
        return std::to_string(unit) + u8"조 " + FormatUnit(remain);
    }
    
    if (val >= 100000000)
    {
        int unit = val / 100000000;
        int remain = val % 100000000;
        return std::to_string(unit) + u8"억 " + FormatUnit(remain);
    }
    
    if (val >= 10000)
    {
        int unit = val / 10000;
        int remain = val % 10000;
        return std::to_string(unit) + u8"만 " + FormatUnit(remain);
    }
    
    return std::to_string(val);
}
