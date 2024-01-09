#include "DownloadManager.h"

#include "curl/curl.h"

#include <filesystem>

#include "../Scene.h"

void DownloadManager::DownloadFile(const std::string& url, const std::string& file_name)
{
    CURL* curl = curl_easy_init();
    if (curl)
    {
        CheckDirectory(file_name.substr(0, file_name.find_last_of('\\')));
        
        FILE* fp = nullptr;
        fopen_s(&fp, file_name.c_str(), "wb");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, ProgressCallback);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        CURLcode res_code = curl_easy_perform(curl);

        fclose(fp);
        
        curl_easy_cleanup(curl);
    }
}

bool DownloadManager::CheckDirectory(const std::string& path)
{
    std::filesystem::path dir(path);
    if (std::filesystem::exists(dir)) return true;
    if (CheckDirectory(dir.parent_path().string()))
        return std::filesystem::create_directory(dir);
    
    return false;
}

size_t DownloadManager::WriteCallback(void* contents, size_t size, size_t nmemb, FILE* userp)
{
    return fwrite(contents, size, nmemb, userp);
}

int DownloadManager::ProgressCallback(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    Scene::GetInstance()->SetProgress(static_cast<float>(dlnow) / static_cast<float>(dltotal));
    return 0;
}
