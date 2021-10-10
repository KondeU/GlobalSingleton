#include "TestDll1.h"
#include "../ExeDllSingleton/TestExe.h"

DLL1_API void TestDll1Run()
{
    GlobalTestExe::GetReference().Test();
}
