#include "APIManager.h"

#include "../Scene.h"
#include "curl/curl.h"

APIManager::APIManager()
{
}

std::string APIManager::UrlEncode(const std::string& str)
{
    CURL* curl = curl_easy_init();
    if (curl)
    {
        char* output = curl_easy_escape(curl, str.c_str(), str.length());
        std::string result(output);
        curl_free(output);
        curl_easy_cleanup(curl);

        return result;
    }

    return "";
}

rapidjson::Document APIManager::Request(const std::string& url)
{
    rapidjson::Document doc;
    CURL* curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res_code = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        
        if (res_code != CURLE_OK) return doc;

        return std::move(doc.Parse(response.c_str()));
    }

    return doc;
}

rapidjson::Document APIManager::RequestAPI(const std::string& api_url)
{
    rapidjson::Document doc;
    CURL* curl = curl_easy_init();
    if (curl)
    {
        std::string url = "https://open.api.nexon.com/maplestory/v1" + api_url;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        
        struct curl_slist* headers = nullptr;
#ifdef _DEBUG
        headers = curl_slist_append(headers, "x-nxopen-api-key: test_0bf77450fb8b1058ad298cc39a29ed3cbc070701fc82eb633a0948ef430d5ec17f653d3c5600e07d31e84750a11f7e19");
#else
        headers = curl_slist_append(headers, "x-nxopen-api-key: live_0bf77450fb8b1058ad298cc39a29ed3cfdd3c90c949584fe41947c380e74f95afe66f4be6304193cd3a13b029251c136");
#endif
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, ProgressCallback);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res_code = curl_easy_perform(curl);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        
        if (res_code != CURLE_OK) return doc;

        return std::move(doc.Parse(response.c_str()));
    }

    return doc;
}

rapidjson::Document APIManager::RequestID(const std::string& character_name)
{
    rapidjson::Document doc = RequestAPI("/id?character_name=" + UrlEncode(character_name));
    return doc;
}

rapidjson::Document APIManager::RequestCharacter(const std::string& ocid, const std::string& date)
{
    rapidjson::Document doc = RequestAPI("/character/basic?ocid=" + ocid + "&date=" + date);
    return doc;
}

rapidjson::Document APIManager::RequestStat(const std::string& ocid, const std::string& date)
{
    rapidjson::Document doc = RequestAPI("/character/stat?ocid=" + ocid + "&date=" + date);
    return doc;
}

rapidjson::Document APIManager::RequestAbility(const std::string& ocid, const std::string& date)
{
    rapidjson::Document doc = RequestAPI("/character/ability?ocid=" + ocid + "&date=" + date);
    return doc;
}

rapidjson::Document APIManager::RequestHyperStat(const std::string& ocid, const std::string& date)
{
    rapidjson::Document doc = RequestAPI("/character/hyper-stat?ocid=" + ocid + "&date=" + date);
    return doc;
}

rapidjson::Document APIManager::RequestSetEffect(const std::string& ocid, const std::string& date)
{
    rapidjson::Document doc = RequestAPI("/character/set-effect?ocid=" + ocid + "&date=" + date);
    return doc;
}

rapidjson::Document APIManager::RequestLinkSkill(const std::string& ocid, const std::string& date)
{
    rapidjson::Document doc = RequestAPI("/character/link-skill?ocid=" + ocid + "&date=" + date);
    return doc;
}

rapidjson::Document APIManager::RequestSkill(const std::string& ocid, const std::string& date,
    const std::string& skill_grade)
{
    rapidjson::Document doc = RequestAPI("/character/skill?ocid=" + ocid + "&date=" + date + "&character_skill_grade=" + skill_grade);
    return doc;
}

rapidjson::Document APIManager::RequestItemEquipment(const std::string& ocid, const std::string& date)
{
    rapidjson::Document doc = RequestAPI("/character/item-equipment?ocid=" + ocid + "&date=" + date);
    return doc;
}

rapidjson::Document APIManager::RequestCashItemEquipment(const std::string& ocid, const std::string& date)
{
    rapidjson::Document doc = RequestAPI("/character/cashitem-equipment?ocid=" + ocid + "&date=" + date);
    return doc;
}

rapidjson::Document APIManager::RequestBeautyEquipment(const std::string& ocid, const std::string& date)
{
    rapidjson::Document doc = RequestAPI("/character/beauty-equipment?ocid=" + ocid + "&date=" + date);
    return doc;
}

rapidjson::Document APIManager::RequestUnionRaider(const std::string& ocid, const std::string& date)
{
    rapidjson::Document doc = RequestAPI("/user/union-raider?ocid=" + ocid + "&date=" + date);
    return doc;
}

size_t APIManager::WriteCallback(char* contents, size_t size, size_t nmemb, std::string* userp)
{
    size_t total_size = size * nmemb;
    userp->append(contents, total_size);
    return total_size;
}

int APIManager::ProgressCallback(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    Scene::GetInstance()->SetProgress(static_cast<float>(dlnow) / static_cast<float>(dltotal));
    return 0;
}
