#pragma once
#include <memory>
#include <mutex>

template <typename T>
class Singleton : public std::enable_shared_from_this<T>
{
    static std::shared_ptr<T> instance_;
    static std::once_flag flag_;

protected:
    Singleton() = default;
    virtual ~Singleton() = default;
    
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static std::shared_ptr<T> GetInstance()
    {
        std::call_once(flag_, []() { instance_.reset(new T()); });
        return instance_;
    }

    virtual void Release()
    {
        instance_.reset();
    }
    
};

template <typename T>
std::shared_ptr<T> Singleton<T>::instance_ = nullptr;

template <typename T>
std::once_flag Singleton<T>::flag_;
