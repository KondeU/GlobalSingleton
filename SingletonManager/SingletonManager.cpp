#include <iostream>
#include <typeinfo>
#include <unordered_map>
#include "../Common/Singleton.hpp"

class SingletonManager : public Singleton<SingletonManager> {
public:
    using TypeId = decltype(typeid(void).hash_code()); //-> size_t

    void* Set(TypeId id, void* instance)
    {
        if (instance) {
            return singletons.emplace(id, instance).first->second;
        }
        return nullptr;
    }

    void* Get(TypeId id)
    {
        auto it = singletons.find(id);
        if (it != singletons.end()) {
            return it->second;
        }
        return nullptr;
    }

    bool Reset(TypeId id)
    {
        return (singletons.erase(id) > 0);
    }

    bool TryReset(TypeId id, void* instance)
    {
        auto it = singletons.find(id);
        if (it != singletons.end() &&
            it->second == instance) {
            singletons.erase(it);
            return true;
        }
        return false;
    }

private:
    std::unordered_map<TypeId, void*> singletons;
};

#ifdef _MSC_VER
#define SGT_API_EXPORT __declspec(dllexport)
#else
#define SGT_API_EXPORT __attribute__((visibility("default")))
#endif

extern "C" {

SGT_API_EXPORT void* SgtSet(SingletonManager::TypeId id, void* instance)
{
    #ifdef _DEBUG
    auto ptr = SingletonManager::GetReference().Set(id, instance);
    std::cout << "SgtSet(" << id << "," << instance << "),return:"
              << ptr << std::endl;
    return ptr; // return!
    #endif
    return SingletonManager::GetReference().Set(id, instance);
}

SGT_API_EXPORT void* SgtGet(SingletonManager::TypeId id)
{
    #ifdef _DEBUG
    auto ptr = SingletonManager::GetReference().Get(id);
    std::cout << "SgtGet(" << id << "),return:"
              << ptr << std::endl;
    return ptr; // return!
    #endif
    return SingletonManager::GetReference().Get(id);
}

SGT_API_EXPORT bool SgtReset(SingletonManager::TypeId id)
{
    #ifdef _DEBUG
    auto suc = SingletonManager::GetReference().Reset(id);
    std::cout << "SgtReset(" << id << "),return:"
              << suc << std::endl;
    return suc; // return!
    #endif
    return SingletonManager::GetReference().Reset(id);
}

SGT_API_EXPORT bool SgtTryReset(SingletonManager::TypeId id, void* instance)
{
    #ifdef _DEBUG
    auto suc = SingletonManager::GetReference().TryReset(id, instance);
    std::cout << "SgtTryReset(" << id << "," << instance << "),return:"
              << suc << std::endl;
    return suc; // return!
    #endif
    return SingletonManager::GetReference().TryReset(id, instance);
}

}
