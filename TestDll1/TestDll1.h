#pragma once

#ifdef _MSC_VER
#ifdef _WINDLL
#define DLL1_API __declspec(dllexport)
#else
#define DLL1_API __declspec(dllimport)
#endif
#else
#define DLL1_API __attribute__((visibility("default")))
#endif

extern "C" {
DLL1_API void TestDll1Run();
}
