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
    
    time_info.tm_mday -= 1;
    mktime(&time_info);

    int year = time_info.tm_year + 1900;
    int month = time_info.tm_mon + 1;
    int day = time_info.tm_mday;

    std::string date = std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);
    return date;
}
