#pragma once

#include "GlobalSingletonContext.hpp"

namespace au {
namespace common {

template <typename T>
class GlobalSingletonInstance final {
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

}
}
