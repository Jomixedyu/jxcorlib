#include <CoreLib.Platform/Window.h>
#include <cassert>

using namespace JxCoreLib::Platform;

void TestPlatform()
{
    auto r = Window::MessageBox("aasd As阿萨德", "b", Window::MessageBoxMode::OkCancel);
    assert(r == Window::MessageBoxResult::Ok);
}