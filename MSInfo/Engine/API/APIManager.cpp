#include "APIManager.h"

#include "httplib/httplib.h"

APIManager::APIManager()
{
}

void APIManager::RequestGetTest()
{
    httplib::Client cli("http://open.api.nexon.com");
    httplib::Headers headers = {
        { "x-nxopen-api-key", "test_0bf77450fb8b1058ad298cc39a29ed3cbc070701fc82eb633a0948ef430d5ec17f653d3c5600e07d31e84750a11f7e19" }
    };

    auto res = cli.Get("/maplestory/v1/id", headers);

    if (res && res->status == 200)
    {
        std::cout << res->body << std::endl;
    }
    else
    {
        std::cout << "error" << std::endl;
    }
}
