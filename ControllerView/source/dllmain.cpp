#include "dllmain.h"

//------------------------------------------------------------------------
/*
void setDllDirectory(LPCWSTR lpDirectory)
{

}
*/

//------------------------------------------------------------------------
void createWindow(HWND parentHwnd, LPCWSTR lpDirectory)
{
    windows = new uiThreadParameters();
    windows->dllDirectory = lpDirectory;
    windows->parentWindow = parentHwnd;
    //windows->threadKeepAlive = TRUE;
    //
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)UI_Thread, (void*)windows, 0, 0);
    //
    //WaitForSingleObjectEx(); // TODO: feedback from UI thread when child window has been created
    //return windows->childWindow;
}

//------------------------------------------------------------------------
void closeWindow()
{
    if (windows->childWindow)
    {
        /*
        BOOL b = DestroyWindow(windows->childWindow);
        if (!b)
        {
            DWORD dwError = GetLastError();
            OutputDebugStringW(L"\ncloseWindow() DestroyWindow(HWND) error = ");
            wchar_t buffer[100];
            wsprintfW(buffer, L"%d", dwError);
            OutputDebugStringW(buffer);
            OutputDebugStringW(L"\n");
        }
        */
        //
        SendMessage(windows->childWindow, WM_CLOSE, 0, 0);
        //
        //Sleep(1000);
        //
        //windows->threadKeepAlive = FALSE;
        //Sleep(1000);
        windows->childWindow = nullptr;
        windows->parentWindow = nullptr;
        windows->dllDirectory = nullptr;
    }
}

//------------------------------------------------------------------------
void resizeWindow(RECT newSize)
{
    //LPARAM lp = *newSize
    //SendMessage(windows->childWindow, WM_SIZING, NULL, lp);
    SetWindowPos(windows->childWindow, // hWnd
                 NULL, // hWndInsertAfter
                 newSize.left, // x
                 newSize.top,  // y
                 (newSize.right  - newSize.left), // width 
                 (newSize.bottom - newSize.top),  // height
                 SWP_NOZORDER // uFlags
                 );
}

//------------------------------------------------------------------------
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    ghInst = hModule;
    //
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            break;
        }
        case DLL_THREAD_ATTACH:
        {
            /*
            HMODULE dll = nullptr;
            if (SYS_WIN64)
            {
                dll = LoadLibraryA("C:\\Program Files\\Common Files\\VST3\\Carlsound\\WebView2Loader.dll");
            }
            else
            {
                dll = LoadLibraryA("C:\\Program Files (x86)\\Common Files\\VST3\\Carlsound\\Win32WebView2Test_ControllerView_x86.dll");
            }
            if (!dll)
            {
                OutputDebugStringW(L"\nLoadLibraryA() Error");
                DWORD dwDllError = GetLastError();
                WCHAR wcDllError[10];
                swprintf_s(wcDllError, 10, L"%i", dwDllError);
                OutputDebugStringW(wcDllError);
                OutputDebugStringW(L"\n");
            }
            */
            break;
        }
        case DLL_THREAD_DETACH:
        {
            break;
        }
        case DLL_PROCESS_DETACH:
        {
            break;
        }
    }
    return TRUE;
}