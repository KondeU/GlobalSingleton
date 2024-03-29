#pragma once

#include "NonCopyable.hpp"

namespace au {
namespace common {

template <typename T>
class Singleton : public NonCopyable {
public:
    static T& GetReference()
    {
        static T instance{};
        return instance;
    }

protected:
    Singleton() = default;
    ~Singleton() = default;
};

}
}
