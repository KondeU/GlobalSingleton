#include "../Common/DllWrapper.hpp"
#include "TestExe.h"

int main(int argc, char* argv[])
{
    DllWrapper dll1, dll2;
    dll1.Load("TestDll1");
    dll2.Load("TestDll2");
    GlobalTestExe::GetReference().Test();
    dll1.ExecuteFunction<void()>("TestDll1Run");
    dll2.ExecuteFunction<void()>("TestDll2Run");
    return 0;
}
