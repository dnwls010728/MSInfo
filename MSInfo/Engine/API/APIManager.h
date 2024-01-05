#pragma once

#include "../Singleton.h"
#include "rapidjson/document.h"

class APIManager : public Singleton<APIManager>
{
public:
    APIManager();
    virtual ~APIManager() final = default;

    std::string UrlEncode(const std::string& str);

    rapidjson::Document Request(const std::string& url);
    rapidjson::Document RequestAPI(const std::string& api_url);
    rapidjson::Document RequestID(const std::string& character_name);
    rapidjson::Document RequestCharacter(const std::string& ocid, const std::string& date);
    rapidjson::Document RequestItemEquip(const std::string& ocid, const std::string& date);
    rapidjson::Document RequestStat(const std::string& ocid, const std::string& date);
    rapidjson::Document RequestAbility(const std::string& ocid, const std::string& date);
    rapidjson::Document RequestHyperStat(const std::string& ocid, const std::string& date);
    rapidjson::Document RequestSetEffect(const std::string& ocid, const std::string& date);
    rapidjson::Document RequestLinkSkill(const std::string& ocid, const std::string& date);
    rapidjson::Document RequestSkill(const std::string& ocid, const std::string& date, const std::string& skill_grade);

private:
    static size_t WriteCallback(char* contents, size_t size, size_t nmemb, std::string* userp);
    
};
