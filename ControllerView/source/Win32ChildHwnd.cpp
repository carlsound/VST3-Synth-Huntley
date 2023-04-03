// ------------------------------------------------------------------------
// Copyright(c) 2021 Carlsound.
//------------------------------------------------------------------------
//
#include "Win32ChildHwnd.h"
//#include "Win32WebView2.h"
//
using namespace Microsoft::WRL;
//
//namespace Carlsound
//{
	//------------------------------------------------------------------------
	//------------------------------------------------------------------------
	HANDLE environmentCreated;
	//HANDLE uiThreadEndEvent;
	HMODULE webView2LoaderDll = nullptr;
	HWND webViewWindow = nullptr;
	LPCWSTR dllDirectoryPath = nullptr;
	WNDCLASSEXW windowClass;
	EventRegistrationToken webMessageReceivedToken = {};
	std::wstring m_sampleUri;
	//
	// Pointer to WebViewController
	static wil::com_ptr<ICoreWebView2Controller> webviewController;
	//
	// Pointer to WebView window
	static wil::com_ptr<ICoreWebView2> webviewWindow;
	//

	//------------------------------------------------------------------------
	//------------------------------------------------------------------------

	HRESULT OnWebMessageReceivedAsync(
		//HRESULT errorCode,
		//ICoreWebView2WebMessageReceivedEventHandler* eventHandler)
		ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args)
	{
		OutputDebugStringW(L"\nWin32ChildHwnd OnWebMessageReceivedAsync() starting...\n");

		wil::unique_cotaskmem_string uri;
		if (!FAILED(args->get_Source(&uri)))
		{
			// Always validate that the origin of the message is what you expect.
			/*
			if (uri.get() != m_sampleUri)
			{
				return S_OK;
			}
			*/
			wil::unique_cotaskmem_string messageRaw;

			if (!FAILED(args->TryGetWebMessageAsString(&messageRaw)))
			{
				std::wstring message = messageRaw.get();

				if (message.compare(0, 22, L"inputSliderHasChanged ") == 0)
				{
					OutputDebugStringW(L"\nWin32ChildHwnd OnWebMessageReceivedAsync() inputSliderHasChanged\n");
					//m_appWindow->SetTitleText(message.substr(13).c_str());
				}
				return S_OK;
			}
			else
			{
				return S_FALSE;
			}
		}
		else
		{
			return S_FALSE;
		}
	}

	//------------------------------------------------------------------------
	
	HRESULT OnControllerCreatedAsync(
		HRESULT errorCode,
		ICoreWebView2Controller* createdController)
	{
		if (!createdController)
		{
			OutputDebugStringW(L"\nWin32ChildHwnd OnCreateControllerCompleted controller is nullptr\n");
		}
		else
		{
			OutputDebugStringW(L"\nWin32ChildHwnd UI_Thread controller is valid\n");
			webViewController = createdController;
			webViewController->get_CoreWebView2(&webView);

			// Add a few settings for the webview
			// The demo step is redundant since the values are the default settings
			ICoreWebView2Settings* Settings;
			webView->get_Settings(&Settings);
			Settings->put_IsScriptEnabled(TRUE);
			Settings->put_AreDefaultScriptDialogsEnabled(TRUE);
			Settings->put_IsWebMessageEnabled(TRUE);

			// Resize WebView to fit the bounds of the parent window
			RECT bounds;
			GetClientRect(webViewWindow, &bounds);
			webViewController->put_Bounds(bounds);

			// Schedule an async task to navigate to Bing
			//webviewWindow->Navigate(L"https://www.bing.com/");
			//webviewWindow->Navigate(L"file:///c:/Users/john_/source/repos/SciterTestDesktopApplication/SciterScriptCallTest.html");
			//webviewWindow->Navigate(L"file:\\\C:\\Users\\john_\\source\\repos\\SciterTestDesktopApplication\\SciterScriptCallTest.html");
			
			std::wstring prefix1(L"file:\\");
			std::wstring prefix2(L"\\");
			std::wstring path(dllDirectoryPath);
			std::wstring filename(L"\\WebView2Test.html");
			std::wstring concatted_stdstr = prefix1 + prefix2 + prefix2 + path + filename;
			LPCWSTR concatted = concatted_stdstr.c_str();
			webView->Navigate(concatted);

			// Step 4 - Navigation events

			// Step 5 - Scripting
			HRESULT hr3 = webView->add_WebMessageReceived
			(
				Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>
				(
					&OnWebMessageReceivedAsync
				).Get(),
				&webMessageReceivedToken
			);
			OutputDebugStringW(L"\nadd_WebMessageReceived HRESULT = ");
			wchar_t buffer[100];
			wsprintfW(buffer, L"%d", hr3);
			OutputDebugStringW(buffer);
			OutputDebugStringW(L"\n");
			

			// Step 6 - Communication between host and web content
		}
		return errorCode;
	}
	

	//------------------------------------------------------------------------
	
	HRESULT OnEnvironmentCreatedAsync(
		HRESULT result,
		ICoreWebView2Environment* environment)
	{
		/*
		HANDLE environmentCreated;
		environmentCreated = CreateEventExW
		(
			NULL,                  //LPSECURITY_ATTRIBUTES lpEventAttributes,
			L"EnvironmentCreated", //LPCWSTR               lpName,
			NULL,                  //DWORD                 dwFlags,
			SYNCHRONIZE            //DWORD                 dwDesiredAccess
		);
		*/

		if (!environment)
		{
			OutputDebugStringW(L"\nWin32ChildHwnd OnEnvironmentCreatedAsync environment is nullptr\n");
			//result = E_FAIL;
		}
		else
		{
			OutputDebugStringW(L"\nWin32ChildHwnd UI_Thread environment is valid\n");
			//
			HRESULT hr2 = environment->CreateCoreWebView2Controller
			(
				webViewWindow,
				Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>
				(
					&OnControllerCreatedAsync
				).Get() 
			);
			OutputDebugStringW(L"\nCreateCoreWebView2Controller HRESULT = ");
			wchar_t buffer[100];
			wsprintfW(buffer, L"%d", hr2);
			OutputDebugStringW(buffer);
			OutputDebugStringW(L"\n");
			//
			//SetEvent(environmentCreated);
		}
		/*
		DWORD dwWaitResult;
		dwWaitResult = WaitForSingleObjectEx
		(
			environmentCreated, //HANDLE hHandle,
			INFINITE,           //DWORD  dwMilliseconds,
			TRUE                //BOOL   bAlertable
		);
		*/
		//
		//OutputDebugStringW(L"\nWin32ChildHwnd OnEnvironmentCreatedAsync -> OnControllerCreatedAsync has returned\n");
		//
		//if (WAIT_OBJECT_0 == dwWaitResult)
		//{
			//OutputDebugStringW(L"\nWin32ChildHwnd OnEnvironmentCreatedAsync -> OnControllerCreatedAsync dwWaitResult was signaled\n");
			return result;
		//}
		/*
		else
		{
			OutputDebugStringW(L"\nWin32ChildHwnd OnEnvironmentCreatedAsync -> OnControllerCreatedAsync dwWaitResult returned unexpectedly\n");
			return S_FALSE;
		}
		*/
	}
	

	//------------------------------------------------------------------------
	//------------------------------------------------------------------------
	DWORD WINAPI UI_Thread(LPVOID lpParam)
	{
		OutputDebugStringW(L"\nUI_Thread\n");
		
		//Carlsound::Win32WebView2* editorView = (Carlsound::Win32WebView2*)lpParam;
		uiThreadParameters* windows = (uiThreadParameters*)lpParam;
		//HWND parentWindow = (HWND)lpParam;
		HWND parentWindow = windows->parentWindow;
		// = childWindow;
		//HWND childWindow;
		//
		dllDirectoryPath = windows->dllDirectory;
		SetDllDirectoryW(dllDirectoryPath);
		/*
		webView2LoaderDll = LoadLibraryA("WebView2Loader.dll");
		if (!webView2LoaderDll)
		{
			OutputDebugStringW(L"\nWebView2Loader.dll was not loaded\n");
			return 0;
		}
		else
		{
			OutputDebugStringW(L"\nWebView2Loader.dll loaded successfully\n");
		*/
			
			RECT  parentWindowRect{ 0 };
			//
			if (parentWindow)
			{
				GetWindowRect(parentWindow, &parentWindowRect);
			}
			//
			windowClass = { 0 };
			windowClass.cbSize = sizeof(WNDCLASSEX);
			windowClass.cbClsExtra = 10;
			windowClass.cbWndExtra = 10;
			windowClass.hCursor = LoadCursor(0, IDC_ARROW);
			windowClass.hIcon = LoadIcon(0, IDI_WINLOGO);
			windowClass.lpszMenuName = NULL;
			windowClass.style = CS_HREDRAW | CS_VREDRAW;;
			windowClass.hbrBackground = 0;
			windowClass.lpfnWndProc = WndProc;
			windowClass.hInstance = ghInst; //m_hinstance;
			windowClass.lpszClassName = TEXT("WebView2");
			//
			ATOM windowClassRegistration = RegisterClassEx(&windowClass);
			//
			if (!windowClassRegistration)
			{
				DWORD dwError = GetLastError();
				OutputDebugStringW(L"\nwindowClassRegistration error = ");
				wchar_t buffer[100];
				wsprintfW(buffer, L"%d", windowClassRegistration);
				OutputDebugStringW(buffer);
				OutputDebugStringW(L"\n");
			}
			else
			{
				webViewWindow = CreateWindowEx(
					windowClass.style,			 // DWORD Optional window styles
					windowClass.lpszClassName,     // LPCSTR lpClassName
					windowClass.lpszClassName,     // LPCSTR lpWindowName
					WS_VISIBLE | WS_CHILD,           // DWORD dwStyle
					0,			// int x start
					0,			// int y start
					parentWindowRect.right - parentWindowRect.left, // int nWidth
					parentWindowRect.bottom - parentWindowRect.top, // int nHeight
					parentWindow,                  // HWND hWndParent
					nullptr,                         // HMENU hMenu
					windowClass.hInstance,         // HINSTANCE hInstance
					NULL);                           // LPVOID lpParam
					//
				if (NULL != webViewWindow)
				{
					OutputDebugStringW(L"\nWin32ChildHwnd childWindow is valid\n");
					windows->childWindow = webViewWindow;
					//
					ShowWindow(webViewWindow, SW_SHOWDEFAULT); //SW_SHOW);
					UpdateWindow(webViewWindow);
					//
					//m_childWindowIsVisible = true;
				}
				else
				{
					OutputDebugStringW(L"\nWin32ChildHwnd childWindow is nullptr\n");
					DWORD dwError = GetLastError();
					OutputDebugStringW(L"\nwindowClassRegistration error = ");
					wchar_t buffer[100];
					wsprintfW(buffer, L"%d", windowClassRegistration);
					OutputDebugStringW(buffer);
					OutputDebugStringW(L"\n");
				}
				//
				/*
				uiThreadEndEvent = CreateEvent(
					NULL,               // default security attributes
					TRUE,               // manual-reset event
					FALSE,              // initial state is nonsignaled
					TEXT("EndEvent")  // object name
				);
				if (uiThreadEndEvent == NULL)
				{
					DWORD dwError = GetLastError();
					printf("CreateEvent failed (%d)\n", dwError);
					return dwError;
				}
				*/

				// Step 3 - Create a single WebView within the parent window
				// Locate the browser and set up the environment for WebView
				CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
				//
				environmentCreated = CreateEventExW
				(
					NULL,                  //LPSECURITY_ATTRIBUTES lpEventAttributes,
					L"EnvironmentCreated", //LPCWSTR               lpName,
					NULL,                  //DWORD                 dwFlags,
					SYNCHRONIZE            //DWORD                 dwDesiredAccess
				);
				//
				
				HRESULT hr1 = CreateCoreWebView2EnvironmentWithOptions
				(
					nullptr,
					L"C:\\Users\\john_\\Documents\\Carlsound_WebView2", //nullptr,
					nullptr,
					Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>
					(
						//this,
						&OnEnvironmentCreatedAsync
						).Get()
				);
				
				//CHAR my_documents[MAX_PATH];
				//HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);
				//


				/*
				HWND hWnd = parentWindow; // webViewWindow;
				HRESULT hr1 = CreateCoreWebView2EnvironmentWithOptions(nullptr, 
					                                                   L"C:\\Users\\john_\\Documents\\Carlsound_WebView2", //nullptr, 
					                                                   nullptr,
					Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
						[hWnd](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {

							// Create a CoreWebView2Controller and get the associated CoreWebView2 whose parent is the main window hWnd
							env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
								[hWnd](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
									if (controller != nullptr) {
										webviewController = controller;
										webviewController->get_CoreWebView2(&webviewWindow);
									}

									// Add a few settings for the webview
									// The demo step is redundant since the values are the default settings
									ICoreWebView2Settings* Settings;
									webviewWindow->get_Settings(&Settings);
									Settings->put_IsScriptEnabled(TRUE);
									Settings->put_AreDefaultScriptDialogsEnabled(TRUE);
									Settings->put_IsWebMessageEnabled(TRUE);

									// Resize WebView to fit the bounds of the parent window
									RECT bounds;
									GetClientRect(hWnd, &bounds);
									webviewController->put_Bounds(bounds);

									// Schedule an async task to navigate to Bing
									webviewWindow->Navigate(L"https://www.bing.com/");

									// Step 4 - Navigation events

									// Step 5 - Scripting

									// Step 6 - Communication between host and web content

									return S_OK;
								}).Get());
							return S_OK;
						}).Get());

				//
				OutputDebugStringW(L"\nCreateCoreWebView2Environment HRESULT = ");
				wchar_t buffer[100];
				wsprintfW(buffer, L"%d", hr1);
				OutputDebugStringW(buffer);
				OutputDebugStringW(L"\n");
				//
				*/
				
				/*
				DWORD dwWaitResult;
				dwWaitResult = WaitForSingleObjectEx
				(
					environmentCreated, //HANDLE hHandle,
					INFINITE,           //DWORD  dwMilliseconds,
					TRUE                //BOOL   bAlertable
				); 
				*/
				
				//
				MSG msg;
				//	
				OutputDebugStringW(L"\nWin32ChildHwnd UI_Thread Message Loop Start\n");
				//while (GetMessage(&msg, editorView->getChildWindow(), 0, 0))
				//while (windows->threadKeepAlive)
				//{
					while (GetMessage(&msg, nullptr, 0, 0))
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
						//OutputDebugStringW(L"\nWin32ChildHwnd UI_Thread Message Loop\n");
					}
				//}
				OutputDebugStringW(L"\nWin32ChildHwnd UI_Thread Message Loop End\n");
				//
				UnregisterClassW(windowClass.lpszClassName, ghInst);
				//
				CoUninitialize();
				//
				return (int)msg.wParam;
			}
		}
	//}

	//------------------------------------------------------------------------
	//------------------------------------------------------------------------
	/*
	void uiThreadEnd()
	{
		SetEvent(uiThreadEndEvent);
	}
	*/

	//------------------------------------------------------------------------
	//------------------------------------------------------------------------
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		OutputDebugStringW(L"\nWin32ChildHwnd UI WndProc\n");
		LRESULT lResult;
		//
		/*
		OutputDebugStringW(L"\nWndProc() message = ");
		wchar_t buffer[100];
		wsprintfW(buffer, L"%i", message);
		OutputDebugStringW(buffer);
		OutputDebugStringW(L"\n");
		*/
		//
		switch (message)
		{
			case WM_NCCREATE:
			{
				OutputDebugStringW(L"\nWin32ChildHwnd UI WndProc WM_NCCREATE\n");
				break;
			}
			case WM_CREATE:
			{
				OutputDebugStringW(L"\nWin32ChildHwnd UI WndProc WM_CREATE\n");
				break;
			}
			case WM_COMMAND:
			{
				OutputDebugStringW(L"\nWin32ChildHwnd UI WndProc WM_COMMAND\n");
				break;
			}
			case WM_PAINT:
			{
				PAINTSTRUCT ps;
				/* HDC hdc = */ 

			
				BeginPaint(hWnd, &ps);
				// TODO: Add any drawing code that uses hdc here...

				EndPaint(hWnd, &ps);
				break;
			}
			/*
			case WM_CLOSE:
			{
				OutputDebugStringW(L"\nWin32ChildHwnd UI WndProc WM_CLOSE\n");
				if (webView2LoaderDll)
				{
					FreeLibrary(webView2LoaderDll);
					webView2LoaderDll = nullptr;
				}
				break;
			}
			*/
			case WM_DESTROY:
			{
				OutputDebugStringW(L"\nWin32ChildHwnd UI WndProc WM_DESTROY\n");
				if (webView2LoaderDll)
				{
					FreeLibrary(webView2LoaderDll);
					webView2LoaderDll = nullptr;
				}
				PostQuitMessage(0);
				break;
			}
			default:
			{
				OutputDebugStringW(L"\nWin32ChildHwnd UI WndProc default\n");
				break;
			}
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	
//------------------------------------------------------------------------
//} // namespace Carlsound