#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <stdexcept>
#include "NonCopyable.hpp"

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#define LIB_LIBRARY          HMODULE
#define LIB_PROCESS          FARPROC
#define LIB_INVALID_LIBRARY  NULL
#define LIB_INVALID_PROCESS  NULL
#define LIB_LOAD(file)       LoadLibraryA(file)
#define LIB_UNLD(inst)       !FreeLibrary(inst)
#define LIB_SYMB(inst, func) GetProcAddress(inst, func)
#define LIB_EXTN             ".dll"
#define LIB_PREN             ""
#else
#include <dlfcn.h>
#define LIB_LIBRARY          void*
#define LIB_PROCESS          void*
#define LIB_INVALID_LIBRARY  nullptr
#define LIB_INVALID_PROCESS  nullptr
#define LIB_LOAD(file)       dlopen(file, RTLD_LAZY)
#define LIB_UNLD(inst)       dlclose(inst)
#define LIB_SYMB(inst, func) dlsym(inst, func)
#define LIB_EXTN             ".so"
#define LIB_PREN             "lib"
#endif

namespace au {
namespace common {

class DllWrapper final : public NonCopyable {
public:
    DllWrapper()
    {
        library = LIB_INVALID_LIBRARY;
    }

    ~DllWrapper()
    {
        UnLoad();
    }

    bool Load(const std::string& file)
    {
        if (library != LIB_INVALID_LIBRARY) {
            return false;
        }

        library = LIB_LOAD((LIB_PREN + file + LIB_EXTN).c_str());
        if (library == LIB_INVALID_LIBRARY) {
            return false;
        }
        return true;
    }

    bool UnLoad()
    {
        if (library == LIB_INVALID_LIBRARY) {
            return true;
        }

        cache.clear();

        if (LIB_UNLD(library)) {
            return false;
        }
        library = LIB_INVALID_LIBRARY;
        return true;
    }

    template <typename FuncAddr>
    FuncAddr* GetFunction(const std::string& function)
    {
        LIB_PROCESS addr = LIB_INVALID_PROCESS;
        auto it = cache.find(function);
        if (it == cache.end()) {
            LIB_PROCESS proc = LIB_SYMB(library, function.c_str());
            it = cache.emplace(function, proc).first;
        }
        addr = it->second;
        return reinterpret_cast<FuncAddr*>(addr);
    }

    template <typename Func, typename ...Args>
    #if (__cplusplus > 201703L) || (_MSVC_LANG > 201703L)
    typename std::invoke_result_t<std::function<Func>, Args...>
    #else
    typename std::result_of<std::function<Func>(Args...)>::type
    #endif
        ExecuteFunction(const std::string& func, Args&& ...args)
    {
        auto fn = GetFunction<Func>(func);
        if (fn == nullptr) {
            std::string except = "Cannot find function: " + func;
            throw std::logic_error(except);
        }
        return fn(std::forward<Args>(args)...);
    }

private:
    LIB_LIBRARY library;
    std::unordered_map<std::string, LIB_PROCESS> cache;
};

}
}
