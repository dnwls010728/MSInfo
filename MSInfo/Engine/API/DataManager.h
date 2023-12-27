#pragma once
#include "../Singleton.h"
#include "rapidjson/document.h"

class DataManager : public Singleton<DataManager>
{
public:
    DataManager();
    virtual ~DataManager() final = default;

    std::string ocid;

    rapidjson::Document character_info;
    
};
