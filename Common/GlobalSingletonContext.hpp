#pragma once

#include "Singleton.hpp"
#include "DllWrapper.hpp"

namespace au {
namespace common {

class GlobalSingletonContext final : public Singleton<GlobalSingletonContext> {
public:
    // singleton manager dynamic library: sgtmgr.dll or (lib)sgtmgr.so
    const std::string SingletonManagerLibrary = "sgtmgr";

    GlobalSingletonContext()
    {
        if (!manager.Load(SingletonManagerLibrary)) {
            std::string except = "The singleton manager library `" +
                SingletonManagerLibrary + "` does not exist!";
            throw std::logic_error(except);
        }
    }

    template <typename T>
    inline T* Regist()
    {
        bool construct = !(manager.ExecuteFunction<uint32_t(size_t)>(
            "Count", typeid(T).hash_code()) > 0);
        T* instance = manager.ExecuteFunction<T*(size_t, size_t)>(
            "Obtain", typeid(T).hash_code(), sizeof(T));
        if (!instance) {
            throw std::logic_error("The singleton internal fatal!");
        }
        if (construct) {
            new(instance) T;
        }
        return instance;
    }

    template <typename T>
    inline void Unregist()
    {
        T* instance = manager.ExecuteFunction<T*(size_t)>(
            "Release", typeid(T).hash_code());
        bool destruct = !(manager.ExecuteFunction<uint32_t(size_t)>(
            "Count", typeid(T).hash_code()) > 0);
        if (instance && destruct) {
            instance->~T();
        }
    }

private:
    DllWrapper manager;
};

}
}
