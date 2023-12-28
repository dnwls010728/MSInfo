#include "APIManager.h"

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

rapidjson::Document APIManager::Request(const std::string& api_url)
{
    rapidjson::Document doc;
    CURL* curl = curl_easy_init();
    if (curl)
    {
        std::string url = "http://open.api.nexon.com/maplestory/v1" + api_url;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "x-nxopen-api-key: test_0bf77450fb8b1058ad298cc39a29ed3cbc070701fc82eb633a0948ef430d5ec17f653d3c5600e07d31e84750a11f7e19");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res_code = curl_easy_perform(curl);
        if (res_code != CURLE_OK) return doc;

        return std::move(doc.Parse(response.c_str()));
    }

    return doc;
}

rapidjson::Document APIManager::RequestID(const std::string& character_name)
{
    rapidjson::Document doc = Request("/id?character_name=" + UrlEncode(character_name));
    return doc;
}

rapidjson::Document APIManager::RequestCharacter(const std::string& ocid, const std::string& date)
{
    rapidjson::Document doc = Request("/character/basic?ocid=" + ocid + "&date=" + date);
    return doc;
}

rapidjson::Document APIManager::RequestItemEquip(const std::string& ocid, const std::string& date)
{
    rapidjson::Document doc = Request("/character/item-equipment?ocid=" + ocid + "&date=" + date);
    return doc;
}

size_t APIManager::WriteCallback(char* contents, size_t size, size_t nmemb, std::string* userp)
{
    size_t total_size = size * nmemb;
    userp->append(contents, total_size);
    return total_size;
}
