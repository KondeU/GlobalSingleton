#pragma once

#include "GlobalSingletonContext.hpp"

template <typename T>
class GlobalSingletonInstance {
public:
    T& GetReference()
    {
        return *instance;
    }

    GlobalSingletonInstance()
    {
        // The regist function makes sure that the instance is not nullptr.
        instance = GlobalSingletonContext::GetReference().Regist<T>();
    }

    ~GlobalSingletonInstance()
    {
        GlobalSingletonContext::GetReference().Unregist<T>();
    }

private:
    T* instance;
};
