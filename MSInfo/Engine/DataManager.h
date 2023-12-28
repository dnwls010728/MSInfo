#pragma once
#include "Singleton.h"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

class DataManager : public Singleton<DataManager>
{
public:
    DataManager();
    virtual ~DataManager() final = default;

    std::string GetDataDate();

    rapidjson::Document id_document;
    rapidjson::Document character_document;
    
};
