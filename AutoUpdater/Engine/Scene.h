#pragma once
#include "Core.h"
#include "Singleton.h"

class Scene : public Singleton<Scene>
{
public:
    Scene();
    virtual ~Scene() final = default;

    void StartUpdate();
    void Tick(float delta_time);
    void Render();

    inline void SetProgress(float progress) { progress_ = progress; }
    inline float GetProgress() const { return progress_; }

private:
    float progress_ = 0.f;

    static DWORD WINAPI DownloadThread(LPVOID lpParam);

    HANDLE thread_handle_ = nullptr;
};
