#pragma once

#include <iostream>
#include "../Common/GlobalSingleton.hpp"

class GlobalTestExe : public GlobalSingleton<GlobalTestExe> {
public:
    void Test()
    {
        calc++;
        std::cout << "GlobalTestExe: Run: " << calc << std::endl;
    }

private:
    int calc = 0;
};
