#include <windows.h>
#include <string.h>
#include "cef_command_line.h"
#include "cef_sandbox_win.h"
#include "SimpleCefApp.h"

// When generating projects with CMake the CEF_USE_SANDBOX value will be defined
// automatically if using the required compiler version. Pass -DUSE_SANDBOX=OFF
// to the CMake command-line to disable use of the sandbox.
// Uncomment this line to manually enable sandbox support.
// #define CEF_USE_SANDBOX 1

#if defined(CEF_USE_SANDBOX)
// The cef_sandbox.lib static library may not link successfully with all VS
// versions.
#pragma comment(lib, "cef_sandbox.lib")
#endif

int APIENTRY wWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine,
    int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Enable High-DPI support on Windows 7 or newer.
    //CefEnableHighDPISupport();

    void* sandbox_info = nullptr;

#if defined(CEF_USE_SANDBOX)
    // Manage the life span of the sandbox information object. This is necessary
    // for sandbox support on Windows. See cef_sandbox_win.h for complete details.
    CefScopedSandboxInfo scoped_sandbox;
    sandbox_info = scoped_sandbox.sandbox_info();
#endif

    // 提供命令行参数
    CefMainArgs main_args(hInstance);

    // CEF应用程序有多个子进程（渲染、插件、GPU)等,它们共享相同的可执行文件。
    // 此函数检查命令行，如果这是一个子进程，则执行相应的逻辑
    int exit_code = CefExecuteProcess(main_args, nullptr, sandbox_info);
    if (exit_code >= 0) {
        return exit_code;
    }

    // 解析此方法中使用的命令行参数
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
    command_line->InitFromString(::GetCommandLineW());

    // CEF全局设置
    CefSettings settings;
    std::string cefPath;

    if (command_line->HasSwitch("enable-chrome-runtime")) {
        //settings.chrome_runtime = true;
    }

#if !defined(CEF_USE_SANDBOX)
    settings.no_sandbox = true;
#endif

    //SimpleApp为浏览器进程实现了应用程序级回调。 
    // 它将在OnContextInitialized（）中创建第一个浏览器实例 
    // CEF已初始化。
    CefRefPtr<SimpleCefApp> app(new SimpleCefApp);

    CefInitialize(main_args, settings, app.get(), sandbox_info);

    // Run the CEF message loop. This will block until CefQuitMessageLoop() is
    // called.
    CefRunMessageLoop();

    // Shut down CEF.
    CefShutdown();

    return 0;
}