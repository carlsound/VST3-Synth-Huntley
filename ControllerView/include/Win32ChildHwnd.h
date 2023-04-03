//------------------------------------------------------------------------
// Copyright(c) 2021 Carlsound.
//------------------------------------------------------------------------
//
#pragma once
//
//#include "Win32WebView2.h"
//
//#include "public.sdk/source/common/pluginview.h"
//#include "base/source/fobject.h"
//#include "pluginterfaces/base/ftypes.h"
//
#include <Windows.h>
#include <WinUser.h>
#include <winuser.h>
//#include <commctrl.h>
//#pragma comment(lib, "comctl32.lib")
#include <windowsx.h>
//
#include <stdlib.h>
#include <string>
#include <tchar.h>
//
//#include "ComponentBase.h"
//#include "Toolbar.h"
//#include "resource.h"
#include <dcomp.h>
#include <functional>
#include <memory>
#include <ole2.h>
#include <string>
#include <vector>
#include <winnt.h>
//
#include <wrl.h>
#include "wil/com.h"
// include WebView2 header
#include "WebView2.h"
//
//------------------------------------------------------------------------
extern HINSTANCE ghInst;
//
//------------------------------------------------------------------------
//namespace Carlsound
//{
	DWORD WINAPI UI_Thread(LPVOID lpParam);
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//
	//void uiThreadEnd();
	//
	/*
	HRESULT OnWebMessageReceivedAsync(HRESULT errorCode, //ICoreWebView2WebMessageReceivedEventHandler* eventHandler);
	  ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args);
	HRESULT OnControllerCreatedAsync(HRESULT errorCode, ICoreWebView2Controller* createdController);
	HRESULT OnEnvironmentCreatedAsync(HRESULT result, ICoreWebView2Environment* environment);
	*/
	//
	//------------------------------------------------------------------------
	struct uiThreadParameters
	{
		LPCWSTR dllDirectory;
		HWND parentWindow;
		HWND childWindow;
		//BOOL threadKeepAlive;
	};
	//
	//wil::com_ptr<ICoreWebView2Environment> webViewEnvironment;
	static wil::com_ptr<ICoreWebView2Controller> webViewController;
	static wil::com_ptr<ICoreWebView2> webView;
//}