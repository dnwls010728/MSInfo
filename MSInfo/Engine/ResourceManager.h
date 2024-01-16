#pragma once
#include <map>

#include "Singleton.h"

class ResourceManager : public Singleton<ResourceManager>
{
public:
    ResourceManager() = default;
    virtual ~ResourceManager() final = default;

    void AddTexture(const std::string& name, const std::string& path);

    inline struct Texture* GetTexture(const std::string& name) { return &textures_[name]; }

private:
    std::map<std::string, struct Texture> textures_;
    
};
