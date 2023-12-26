#pragma once

#include "../Singleton.h"

#include <windows.h>

class Time : public Singleton<Time>
{
public:
    Time();
    virtual ~Time() final = default;
    
    void Init();
    void Tick();

    inline float GetDeltaTime() const { return delta_time_; }
    inline float GetFPS() const { return fps_; }

private:
    LARGE_INTEGER frequency_;
    LARGE_INTEGER previous_count_;
    LARGE_INTEGER current_count_;

    float delta_time_;
    float frame_count_;
    float frame_timer_;
    float fps_;
    
};
