#pragma once

#include "GlobalSingletonRegister.hpp"

template <typename T>
class GlobalSingleton : public Singleton<GlobalSingleton<T>> {
public:
    static T& GetReference()
    {
        return *instance;
    }

protected:
    GlobalSingleton()
    {
        instance = GlobalSingletonRegister::GetReference().Regist(
            Singleton<T>::GetReference());
    }

    virtual ~GlobalSingleton()
    {
        GlobalSingletonRegister::GetReference().Unregist(
            Singleton<T>::GetReference());
    }

private:
    static T* instance;
};

template <typename T>
T* GlobalSingleton<T>::instance;
