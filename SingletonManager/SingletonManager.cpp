#include <mutex>
#include <typeinfo>
#include <functional>
#include <unordered_map>
#include "../Common/Singleton.hpp"

class SingletonManager : public Singleton<SingletonManager> {
public:
    using TypeId = decltype(typeid(void).hash_code()); //-> size_t

    void* Set(TypeId id, size_t size, std::function<void(void*)> constructor)
    {
        std::lock_guard<std::mutex> locker(mutex);
        std::vector<uint8_t>& data = singletons[id];
        if (data.size() <= 0) {
            data.resize(size);
            constructor(data.data());
        }
        return data.data();
    }

    bool Reset(TypeId id, std::function<void(void*)> destructor)
    {
        std::lock_guard<std::mutex> locker(mutex);
        const auto& iter = singletons.find(id);
        if (iter != singletons.end()) {
            destructor(iter->second.data());
            singletons.erase(iter);
            return true;
        }
        return false;
    }

private:
    std::mutex mutex;
    std::unordered_map<TypeId, std::vector<uint8_t>> singletons;
};

#ifdef _MSC_VER
#define SGT_API_EXPORT __declspec(dllexport)
#else
#define SGT_API_EXPORT __attribute__((visibility("default")))
#endif

extern "C" {

SGT_API_EXPORT void* SgtSet(size_t id, size_t size, void(*constructor)(void*))
{
    return SingletonManager::GetReference().Set(id, size, constructor);
}

SGT_API_EXPORT bool SgtReset(size_t id, void(*destructor)(void*))
{
    return SingletonManager::GetReference().Reset(id, destructor);
}

}
