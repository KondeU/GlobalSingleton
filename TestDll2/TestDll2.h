#pragma once

#ifdef _MSC_VER
#ifdef _WINDLL
#define DLL2_API __declspec(dllexport)
#else
#define DLL2_API __declspec(dllimport)
#endif
#else
#define DLL2_API __attribute__((visibility("default")))
#endif

extern "C" {
DLL2_API void TestDll2Run();
}
