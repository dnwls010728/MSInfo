#pragma once
#include "../Singleton.h"

class DownloadManager : public Singleton<DownloadManager>
{
public:
    void DownloadFile(const std::string& url, const std::string& file_name);

private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, FILE* userp);
    
};
