#include "TestDll2.h"
#include "../ExeDllSingleton/TestExe.h"

DLL2_API void TestDll2Run()
{
    GlobalTestExe::GetReference().Test();
}
