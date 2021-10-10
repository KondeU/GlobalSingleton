#pragma once

#include "Singleton.hpp"
#include "DllWrapper.hpp"

class GlobalSingletonRegister : public Singleton<GlobalSingletonRegister> {
public:
    // singleton_manager.dll or singleton_manager.so
    const std::string SingletonManagerLibrary = "singleton_manager";

    GlobalSingletonRegister()
    {
        if (!sgtManer.Load(SingletonManagerLibrary)) {
            std::string except = "The singleton manager library `" +
                SingletonManagerLibrary + "` does not exist!";
            throw std::exception(except.c_str());
        }
    }

    template <typename T>
    inline T* Regist(T& reference)
    {
        return sgtManer.ExecuteFunction<T*(size_t, T*)>(
            "SgtSet", typeid(T).hash_code(), &reference);
    }

    template <typename T>
    inline bool Unregist(T& reference)
    {
        return sgtManer.ExecuteFunction<bool(size_t, T*)>(
            "SgtTryReset", typeid(T).hash_code(), &reference);
    }

    template <typename T>
    inline T* Obtain()
    {
        return sgtManer.ExecuteFunction<T*(size_t)>(
            "SgtGet", typeid(T).hash_code());
    }

private:
    DllWrapper sgtManer;
};

template <typename T>
class GlobalSingleton : public Singleton<T> {
public:
    static T* GetSafetyPtr()
    {
        instance = GlobalSingletonRegister::GetReference().Obtain();
        return instance;
    }

    static T& GetSafetyRef()
    {
        instance = GlobalSingletonRegister::GetReference().Obtain();
        if (!instance) {
            throw std::exception("Global singleton instance is null!");
        }
        return *instance;
    }

    static T& GetReference()
    {
        if (!instance) {
            throw std::exception("Global singleton local instance is null!");
        }
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
