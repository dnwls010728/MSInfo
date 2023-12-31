#include "DataManager.h"

#include <algorithm>
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

std::string DataManager::FormatUnit(int value)
{
    if (value >= 100000000)
    {
        int unit = value / 100000000;
        int remain = value % 100000000;
        return std::to_string(unit) + u8"억 " + FormatUnit(remain);
    }
    
    if (value >= 10000)
    {
        int unit = value / 10000;
        int remain = value % 10000;
        return std::to_string(unit) + u8"만 " + FormatUnit(remain);
    }
    
    return std::to_string(value);
}

std::string DataManager::FormatComma(long value)
{
    std::string str = std::to_string(value);
    int count = 0;
    
    for (int i = str.length() - 1; i >= 0; i--)
    {
        count++;
        if (count == 3 && i != 0)
        {
            str.insert(i, ",");
            count = 0;
        }
    }

    return str;
}

std::string DataManager::SafeFormatUnit(std::string value)
{
    const bool is_digit = std::all_of(value.begin(), value.end(), ::isdigit);
    if (value.empty() || !is_digit) return value;
    return FormatUnit(std::stoi(value));
}

std::string DataManager::SafeFormatComma(std::string value)
{
    const bool is_digit = std::all_of(value.begin(), value.end(), ::isdigit);
    if (value.empty() || !is_digit) return value;
    return FormatComma(std::stol(value));
}
