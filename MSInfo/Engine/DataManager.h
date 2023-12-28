#pragma once
#include <vector>

#include "Singleton.h"
#include "rapidjson/document.h"
#include "ItemEquip.h"

class DataManager : public Singleton<DataManager>
{
public:
    DataManager();
    virtual ~DataManager() final = default;

    std::string GetDataDate();

    rapidjson::Document id_document;
    rapidjson::Document character_document;

    std::vector<std::unique_ptr<ItemEquip>> items;
    
};
