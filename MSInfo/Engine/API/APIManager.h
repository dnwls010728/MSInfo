#pragma once
#include "../Core.h"
#include "../Singleton.h"

class APIManager : public Singleton<APIManager>
{
public:
    APIManager();
    virtual ~APIManager() final = default;

private:
    
};
