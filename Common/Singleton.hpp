#pragma once

#include "NonCopyable.hpp"

template <typename T>
class Singleton : public NonCopyable {
public:
    static T& GetReference()
    {
        return instance;
    }

protected:
    Singleton() = default;
    ~Singleton() = default;

private:
    static T instance;
};

template <typename T>
T Singleton<T>::instance;
