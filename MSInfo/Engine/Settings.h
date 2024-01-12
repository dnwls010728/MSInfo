#pragma once
#include "Singleton.h"

class Settings : public Singleton<Settings>
{
public:
    Settings();
    virtual ~Settings() final = default;

    void WriteFile(const std::string& key, const std::string& value);
    std::string ReadFile(const std::string& key);
    
};
