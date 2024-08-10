#pragma once

#include "GlobalSingletonContext.hpp"

namespace au {
namespace common {

template <typename T>
class GlobalSingletonInstance final {
public:
    GlobalSingletonInstance()
    {
        // The regist function makes sure that the instance is not nullptr.
        instance = GlobalSingletonContext::GetReference().Regist<T>();
    }

    size_t DecreaseCounter()
    {
        // The unregist function will decrease global singleton reference count,
        // if decrease to 0, it will call global singleton instance destructor.
        // Return value is current global singleton reference count.
        // If you need to hot-unload dynamic libraries, you should manually
        // manage the counter to prevent memory leaks, then you need call
        // this function.
        return GlobalSingletonContext::GetReference().Unregist<T>();
    }

    void ForceRelease()
    {
        // Force decrease count to 0 and call destructor.
        while (DecreaseCounter());
    }

    T& GetReference()
    {
        return *instance;
    }

private:
    T* instance;
};

}
}
