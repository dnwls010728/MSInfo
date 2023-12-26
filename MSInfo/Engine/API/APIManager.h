#pragma once
#include "../Singleton.h"

class APIManager : public Singleton<APIManager>
{
public:
    APIManager();
    virtual ~APIManager() final = default;

private:
    
};
