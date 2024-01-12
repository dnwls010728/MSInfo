#include "Settings.h"

#include <Windows.h>

Settings::Settings()
{
}

void Settings::WriteFile(const std::string& key, const std::string& value)
{
    WritePrivateProfileStringA("Settings", key.c_str(), value.c_str(), ".\\MSInfo.ini");
}

std::string Settings::ReadFile(const std::string& key)
{
    char buffer[256];
    GetPrivateProfileStringA("Settings", key.c_str(), "", buffer, 256, ".\\MSInfo.ini");
    return buffer;
}
