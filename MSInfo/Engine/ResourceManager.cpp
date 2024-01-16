#include "ResourceManager.h"

#include "Texture.h"
#include "Graphics/Graphics.h"
#include "imgui/imgui.h"

#define RESOURCES ".\\Resources"

void ResourceManager::AddTexture(const std::string& name, const std::string& path)
{
    struct Texture texture;
    bool ret = Graphics::GetInstance()->LoadTexture(RESOURCES + path, &texture.texture, &texture.width, &texture.height);
    IM_ASSERT(ret);

    textures_[name] = texture;
}
