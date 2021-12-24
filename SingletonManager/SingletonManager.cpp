#include <typeinfo>
#include <unordered_map>

#ifdef _MSC_VER
#define SGT_API_EXPORT __declspec(dllexport)
#else
#define SGT_API_EXPORT __attribute__((visibility("default")))
#endif

namespace {
using type = decltype(typeid(void).hash_code()); //-> size_t
std::unordered_map<type, std::pair<std::vector<uint8_t>, uint32_t>> singletons;
}

extern "C" {
SGT_API_EXPORT uint32_t Count(size_t id)
{
    return singletons[id].second;
}

SGT_API_EXPORT void* Obtain(size_t id, size_t size)
{
    auto& singleton = singletons[id];
    auto& sgtDatBin = singleton.first;
    auto& sgtRefCnt = singleton.second;
    if (sgtDatBin.size() > 0) {
        if (sgtDatBin.size() != size) {
            return nullptr;
        }
    } else {
        sgtDatBin.resize(size);
    }
    sgtRefCnt++;
    return sgtDatBin.data();
}

SGT_API_EXPORT void* Release(size_t id)
{
    auto& singleton = singletons[id];
    auto& sgtDatBin = singleton.first;
    auto& sgtRefCnt = singleton.second;
    if (!(sgtRefCnt > 0)) {
        return nullptr;
    }
    sgtRefCnt--;
    return sgtDatBin.data();
}
}
