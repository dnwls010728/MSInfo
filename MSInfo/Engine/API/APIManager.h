#pragma once
#include "../Singleton.h"

class APIManager : public Singleton<APIManager>
{
public:
    APIManager();
    virtual ~APIManager() final = default;
    
    std::string UrlEncode(const std::string& str);
    std::string Request(const std::string& api_url);

private:
    static size_t WriteCallback(char* contents, size_t size, size_t nmemb, std::string* userp);
    
};
