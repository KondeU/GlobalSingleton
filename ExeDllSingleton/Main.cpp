#include "../Common/DllWrapper.hpp"
#include "TestExe.h"

/// 支持热加载和热卸载的单例 - 测试示例程序
/// Windows使用注意事项：
// 1. 要求使用MD/MDd，而不是MT/MTd，保证exe和所有dll间使用同一个crtheap堆，否则
//    exe和不同dll间都有各自的crtheap堆，而new和delete需要在同一堆中配套执行。
// 2. 慎用virtual，析构函数不能为virtual函数。因为当析构函数为virtual函数时，如果
//    单例由dll1加载，而dll2在dll1卸载之后如果还在使用，那析构时调virtual的析构函
//    数会去查虚函数表，而虚函数表是由dll1创建的，会引发崩溃。其他虚函数也涉及虚函
//    数表，因此若要使用虚函数，那么除非能保证dll的卸载顺序，否则不要使用热卸载。
//    事实上，全局单例的管理交由单例框架来实现后，析构函数是否使用virtual都不会产生
//    泄漏，因为单例框架构造和析构时使用的都是具体的全局单例类，而不会是它们的基类。
// 3. 单例全为懒加载，直到GetReference的时候才真正实例化单例对象，需要注意全局单例
//    没有保证多线程间安全，因此在单例实例化/获取单例时都是线程不安全的。
//    若在SingletonManager.cpp的Count/Obtain/Release函数中使用std::mutex加锁能够实现
//    单例获取的安全，但实例化过程(创建过程)仍是线程不安全的。

int main(int argc, char* argv[])
{
    //GlobalTestExe::GetReference().Test(); // 单例由exe加载

    DllWrapper dll1, dll2;
    dll1.Load("TestDll1"); // 单例由dll加载
    dll2.Load("TestDll2");
    dll1.ExecuteFunction<void()>("TestDll1Run");
    dll2.ExecuteFunction<void()>("TestDll2Run");

    // 用于测试：
    // 析构不能为virtual；
    // 如果函数不想关注dll的加载和卸载顺序，该函数不能为virtual
    //dll1.UnLoad();

    // 用于测试如果该函数设置为virtual，而第一次加载者为dll1
    // 而不是dll2，此时dll1已卸载，调用虚函数会出现崩溃
    //dll2.ExecuteFunction<void()>("TestDll2Run");

    GlobalTestExe::GetReference().Test();
    return 0;
}
