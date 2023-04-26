#include <CoreLib.Platform/Window.h>
#include <CoreLib.Platform/FolderWatch.h>

#include <cassert>
#include <string>

using namespace jxcorlib::platform;
using namespace std;

void TestPlatform()
{
    //auto win = Window::MainWindow();

    //auto r = Window::MessageBox(win,"aasd As阿萨德", "b", Window::MessageBoxMode::OkCancel);
    //assert(r == Window::MessageBoxResult::Ok);

    //string sel;
    //Window::OpenFileDialog(0, "ExeFile(*.exe)|*.exe", "C:\\", &sel);
    //int a = 3;

    FolderWatch w{ "D:\\newfolder", true };
    w.Tick();
    w.Tick();
    w.Tick();

}