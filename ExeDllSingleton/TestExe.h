#pragma once

#include <iostream>
#include "../Common/GlobalSingleton.hpp"

class GlobalTestExe : public GlobalSingleton<GlobalTestExe> {
public:
    GlobalTestExe()
    {
        pb = malloc(100);
    }

    /*virtual */~GlobalTestExe()
    {
        free(pb);
        pb = nullptr;
    }

    virtual void Test()
    {
        calc++;
        std::cout << "GlobalTestExe: Run: " << calc << std::endl;
    }

private:
    void* pb = nullptr;
    int calc = 0;
};
