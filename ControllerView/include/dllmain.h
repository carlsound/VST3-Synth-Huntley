#pragma once
//
#include <windows.h>
//
#include "Win32ChildHwnd.h"
//
//------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {  // only need to export C interface if
              // used by C++ source code
#endif

//__declspec(dllexport) void setDllDirectory(LPCWSTR lpDirectory);
__declspec(dllexport) void createWindow(HWND parentHwnd, LPCWSTR lpDirectory);
__declspec(dllexport) void closeWindow();
__declspec(dllexport) void resizeWindow(RECT newSize);

#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------
uiThreadParameters* windows;
HINSTANCE ghInst;

//------------------------------------------------------------------------
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved);