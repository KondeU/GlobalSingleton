#pragma once

#include "Singleton.hpp"
#include "DllWrapper.hpp"

class GlobalSingletonRegister : public Singleton<GlobalSingletonRegister> {
public:
    // singleton manager dynamic library: sgtmgr.dll or (lib)sgtmgr.so
    const std::string SingletonManagerLibrary = "sgtmgr";

    GlobalSingletonRegister()
    {
        if (!sgtMgr.Load(SingletonManagerLibrary)) {
            std::string except = "The singleton manager library `" +
                SingletonManagerLibrary + "` does not exist!";
            throw std::exception(except.c_str());
        }
    }

    template <typename T>
    inline T* Regist(T& reference)
    {
        return sgtMgr.ExecuteFunction<T*(size_t, size_t, void*)>(
            "SgtSet", typeid(T).hash_code(), sizeof(T), Constructor<T>);
    }

    template <typename T>
    inline bool Unregist(T& reference)
    {
        return sgtMgr.ExecuteFunction<bool(size_t, void*)>(
            "SgtReset", typeid(T).hash_code(), Destructor<T>);
    }

private:
    template <typename T>
    static void Constructor(void* instance)
    {
        new(instance) T; // instance->T()
    }

    template <typename T>
    static void Destructor(void* instance)
    {
        static_cast<T*>(instance)->~T();
    }

    DllWrapper sgtMgr;
};
