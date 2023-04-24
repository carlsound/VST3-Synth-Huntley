// ------------------------------------------------------------------------
// Copyright(c) 2021 Carlsound.
//------------------------------------------------------------------------
//
#include "MacWKWebView.h"
//
#include "bundleID.h"
//
#include <dlfcn.h> // dlopen(), dlsym()
#include <mach-o/dyld.h>
#include <CoreFoundation/CoreFoundation.h>
//
#include <iostream>
//#include <filesystem>
//
//#import <Cocoa/Cocoa.h>
//
//------------------------------------------------------------------------
typedef void (*print_t)();
//------------------------------------------------------------------------
typedef int (*dllCreateSubviewFunction)(void* view); //LPCWSTR dllDirectory);
typedef int (*dllCloseSubviewFunction)();
typedef int (*dllResizeSubviewFunction)(void* newSize); //(Steinberg::ViewRect* newSize);
//------------------------------------------------------------------------
//
namespace Carlsound
{
	//------------------------------------------------------------------------
    //Steinberg::Vst::EditController * m_controller {nullptr};
    IPlugViewLifetimeEventHandler* m_lifetimeEventHandler = nullptr;
    std::shared_ptr<Steinberg::ViewRect> m_rect;
    void* m_systemWindow {nullptr};
    Steinberg::IPlugFrame* m_plugFrame = nullptr;
    //
    //NSView* m_parentWindow = nullptr;
    //HWND m_childWindow  = nullptr;
    //uiThreadParameters* m_windows;
    //RECT  m_parentWindowRect { 0 };
    //
    bool m_childWindowIsVisible = false;
    //
    //HMODULE*   m_hmodule   = nullptr;
    //HINSTANCE* m_hinstance = nullptr;
    //
    //HMODULE m_webView2LoaderDll = nullptr;
    //HMODULE m_controllerviewDll = nullptr;
    //
    //HANDLE m_uiThread;
    //
    std::string dylibPath;
    //
    dllCreateSubviewFunction m_dllCreateSubviewFunction = nullptr;
    dllCloseSubviewFunction  m_dllCloseSubviewFunction  = nullptr;
    dllResizeSubviewFunction m_dllResizeSubviewFunction = nullptr;

	// Win32WebView2 Implementation
	//------------------------------------------------------------------------
	MacWKWebView::MacWKWebView()
	{
        std::cout << "\nMacWebView::MacWebView()\n";
		m_rect = std::make_shared<Steinberg::ViewRect>(0, 0, 500, 400);
		//
		//m_windows = new uiThreadParameters();
		//
		m_plugFrame = this;
	}

	//------------------------------------------------------------------------
	MacWKWebView::~MacWKWebView()
	{
        fprintf(stderr, "\nMacWKWebView::~MacWebView()\n");
		//closeWindow();
		//
		m_lifetimeEventHandler = nullptr;
		//
		//m_hmodule = nullptr;
		//m_hinstance = nullptr;
	}

	//------------------------------------------------------------------------
	void MacWKWebView::closeWindow()
	{
        fprintf(stderr, "\nMacWKWebView::closeWindow()\n");
		if (m_childWindowIsVisible)
		{
			m_childWindowIsVisible = false;
			//uiThreadEnd();
		}
		/*
		if (m_windows->childWindow)
		{
			//SendMessage(m_windows.childWindow, WM_QUIT, 0, 0);
			//
			BOOL b = DestroyWindow(m_windows->childWindow);
			m_windows->childWindow = nullptr;
		}
		*/
		// TODO: add call to dll closeWindow() exported function
        /*
         if (m_controllerviewDll) // TODO
         {
             NSLog(@"\nChecking DLL for closeWindow() function...\n");
             m_dllCloseChildWindowFunction = (void (*) () ) GetProcAddress(m_controllerviewDll, "closeWindow");
             if (m_dllCloseChildWindowFunction)
             {
                 OutputDebugStringW(L"\nCalling DLL closeWindow() function\n");
                 m_dllCloseChildWindowFunction();
             }
             //
             m_dllCreateChildWindowFunction = nullptr;
             m_dllCloseChildWindowFunction = nullptr;
             m_dllResizeChildWindowFunction = nullptr;
             //
             FreeLibrary(m_controllerviewDll);
             m_controllerviewDll = nullptr;
         }
         */
		
		/*
		if (m_webView2LoaderDll)
		{
			FreeLibrary(m_webView2LoaderDll);
			m_webView2LoaderDll = nullptr;
		}
		*/
	}

	//------------------------------------------------------------------------
	void MacWKWebView::attachedToParent()
	{
        std::cout << "\nMacWKWebView::attachedToParent()\n";
		// Create the window.
		/*
		if (m_controller)
		{
			m_controller->editorAttached(this);
		}
		*/
        //
		if (m_lifetimeEventHandler)
		{
			m_lifetimeEventHandler->viewAttached(this);
		}
		//
        std::cout << "\nMacWKWebView::attachedToParent() CreateThread()\n";
        //
        void *lib_handle;
        print_t printTC;
        //
        //
        //
        // https://developer.apple.com/library/archive/documentation/CoreFoundation/Conceptual/CFBundles/AccessingaBundlesContents/AccessingaBundlesContents.html
        CFBundleRef mainBundle;
        // Get the main bundle for the app
        mainBundle = CFBundleGetMainBundle();
        //
        CFBundleRef requestedBundle;
         // Look for a bundle using its identifier
         requestedBundle = CFBundleGetBundleWithIdentifier(
                //CFSTR("net.carlsound.01.Test.NSView.Mac") );
                CFSTR(BUNDLE_IDENT) );
        //
        CFURLRef resourceURL = CFBundleCopyBundleURL(requestedBundle); //CFBundleCopyResourcesDirectoryURL(requestedBundle);
          char resourcePath[PATH_MAX];
          if (CFURLGetFileSystemRepresentation(resourceURL, true,
                                               (UInt8 *)resourcePath,
                                               PATH_MAX))
          {
            if (resourceURL != NULL)
            {
                CFRelease(resourceURL);
                std::string prefix = "file://";
                dylibPath = resourcePath;
                //dylibPath = dylibPath.substr( prefix.length() );
                //dylibPath.erase(0, prefix.length());
                dylibPath = dylibPath + "/Contents/Frameworks/Huntley_ControllerView.framework/Versions/A/Huntley_ControllerView"; // "/Contents/Frameworks/libMac_ControllerView_Test_01.dylib";
                lib_handle = dlopen(dylibPath.c_str(), RTLD_NOW);
            }
          }
        //
        //
        //
        //lib_handle = dlopen("/Users/johncarlson/Documents/XCode/VST3_build/VST3/Debug/Mac_NSView_Test_01.vst3/Contents/Frameworks/Mac_ControllerView_Test_01.framework/Mac_ControllerView_Test_01", RTLD_NOW);
        //lib_handle = dlopen("/Users/johncarlson/Library/Audio/Plug-Ins/VST3/Mac_NSView_Test_01.vst3/Contents/Frameworks/libMac_ControllerView_Test_01.dylib", RTLD_NOW);
        //
        if (lib_handle == NULL) {
                // error handling
            const char* dlsym_error = dlerror();
                if (dlsym_error) {
                    std::cerr << "Cannot load symbol create: " << dlsym_error << '\n';
                    //return 1;
                }
            std::cout << "\n\nERROR: lib_handle == NULL\n\n";
            }
        else{
            std::cout << "\n\nSucess for step 1: lib_handle exists!!!!\n\n";
            //
            //
            /*
            printTC = (print_t) dlsym(lib_handle, "printToConsoleObjC");
            if(printTC == NULL)
            {
                std::cout << "\n\nERROR: printToConsoleObjC == NULL\n\n";
                //
                const char* err = dlerror();
                printf("%s", err);
                int i = 0;
            }
            else{
                std::cout << "\n\nSucess for step 2: printToConsoleObjC() exists within lib_handle!!!!\n\n";
                //
                printTC();
                int j = 0;
            }
            */
            //
            m_dllCloseSubviewFunction = (dllCloseSubviewFunction) dlsym(lib_handle, "closeSubview");
            if(m_dllCloseSubviewFunction == NULL)
            {
                char* err = dlerror();
                int i = 0;
            }
            else
            {
                std::cout << "\n\nSucess for step 2: closeSubview() exists within lib_handle!!!!\n\n";
                //
                int j = 0;
            }
            //
            dlerror();
            m_dllCreateSubviewFunction = (dllCreateSubviewFunction) dlsym(lib_handle, "createSubview");
            if(m_dllCreateSubviewFunction == NULL)
            {
                char* err = dlerror();
                int i = 0;
            }
            else
            {
                std::cout << "\n\nSucess for step 2: createSubview(void* view) exists within lib_handle!!!!\n\n";
                //
                m_dllCreateSubviewFunction(m_systemWindow);
                int j = 0;
            }
        }
		//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)UI_Thread, (void*)m_windows, 0, 0);
		//
		//
		//
		// https://stackoverflow.com/questions/6924195/get-dll-path-at-runtime
		//LPWSTR lpPath = new WCHAR[MAX_PATH]; //[MAX_PATH];
		/*
		* HMODULE hm = NULL;
		if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
			GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
			(LPCWSTR)&InitDll, &hm) == 0)
		{
			int ret = GetLastError();
			fprintf(stderr, "GetModuleHandle failed, error = %d\n", ret);
			// Return or however you want to handle an error.
		}
		*/
		
		//
        /*
         DWORD dwDllFilePath = GetModuleFileName(ghInst, lpPath, MAX_PATH /* sizeof(lpPath) */ //);
        /*
         if (dwDllFilePath == 0)
         {
             int ret = GetLastError();
             fprintf(stderr, "GetModuleFileName failed, error = %d\n", ret);
             // Return or however you want to handle an error.
         }
         // The path variable should now contain the full filepath for this DLL.
         PathCchRemoveFileSpec(lpPath, MAX_PATH);
         OutputDebugStringW(L"\nDLL path = ");
         OutputDebugStringW(lpPath);
         OutputDebugStringW(L"\n");
         //
         SetDllDirectoryW(lpPath);
         //
         //
         if (SMTG_PLATFORM_64)
         {
             //m_webView2LoaderDll = LoadLibraryA("C:\\Program Files\\Common Files\\VST3\\Carlsound\\WebView2Loader.dll");
             //if (m_webView2LoaderDll)
             //{
                 //m_controllerviewDll = LoadLibraryA("C:\\Program Files\\Common Files\\VST3\\Carlsound\\Win32WebView2Test_ControllerView_x64.dll");
             m_controllerviewDll = LoadLibraryA("Win32WebView2Test_ControllerView_x64.dll");
             //}
             
         }
         else
         {
             //m_webView2LoaderDll = LoadLibraryA("C:\\Program Files (x86)\\Common Files\\VST3\\Carlsound\\Win32WebView2Test_ControllerView_x86.dll");
             //if (m_webView2LoaderDll)
             //{
                 //m_controllerviewDll = LoadLibraryA("C:\\Program Files (x86)\\Common Files\\VST3\\Carlsound\\Win32WebView2Test_ControllerView_x86.dll");
                 m_controllerviewDll = LoadLibraryA("Win32WebView2Test_ControllerView_x86.dll");
             //}
         }
         if (!m_controllerviewDll)
         {
             OutputDebugStringW(L"\nLoadLibraryA() Error");
             DWORD dwDllError = GetLastError();
             WCHAR wcDllError[10];
             swprintf_s(wcDllError, 10, L"%i", dwDllError);
             OutputDebugStringW(wcDllError);
             OutputDebugStringW(L"\n");
         }
         else
         {
             OutputDebugStringW(L"\nChecking DLL for createWindow(HWND) function...\n");
             m_dllCreateChildWindowFunction = (void (*) (HWND, LPCWSTR)) GetProcAddress(m_controllerviewDll, "createWindow");
             if (m_dllCreateChildWindowFunction)
             {
                 OutputDebugStringW(L"\nCalling DLL createWindow(HWND) function\n");
                 m_dllCreateChildWindowFunction(m_parentWindow, lpPath);
             }
         }
         //
         m_childWindowIsVisible = true;
         //OutputDebugStringW(L"\nattachedToParent()\n");
         */
        
		
	}

	//------------------------------------------------------------------------
	void MacWKWebView::removedFromParent()
	{
        std::cout << "\nMacWKWebView::removedFromParent()\n";
		//
		m_childWindowIsVisible = false;
		//
		/*
		if (m_controller)
		{
			m_controller->editorRemoved(this);
		}
		*/
		closeWindow();
		//
		if (m_lifetimeEventHandler)
		{
			m_lifetimeEventHandler->viewRemoved(this);
		}
		//
		//m_childWindowIsVisible = false;
		//m_parentWindow = nullptr;
		m_systemWindow = nullptr;
		m_plugFrame = nullptr;
		//
		if (m_rect)
		{
			m_rect = nullptr;
		}
	}

	//------------------------------------------------------------------------
	//------------------------------------------------------------------------
	Steinberg::tresult PLUGIN_API MacWKWebView::isPlatformTypeSupported(Steinberg::FIDString type)
	{
        std::cout << "\nMacWKWebView::isPlatformTypeSupported(Steinberg::FIDString type)\n";
		if (strcmp(type, Steinberg::kPlatformTypeNSView) == 0)
		{
            std::cout << "\nMacWKWebView::isPlatformTypeSupported type = NSView\n";
			return Steinberg::kResultTrue;
		}
		else
		{
			return Steinberg::kResultFalse;
		}
	}

	//------------------------------------------------------------------------
	Steinberg::tresult PLUGIN_API MacWKWebView::attached(void* parent, Steinberg::FIDString type)
	{
        std::cout << "\nMacWebView::attached(void* parent, Steinberg::FIDString type)\n";
		if (strcmp(type, Steinberg::kPlatformTypeNSView) == 0)
		{
            std::cout << "\nSteinberg::kPlatformTypeNSView\n";
            //
			m_systemWindow = parent;
			//m_parentWindow = (NSView*)parent;
			//m_windows->parentWindow = (HWND)parent;
			//
			/*
			if (m_plugFrame != nullptr)
			{
				m_plugFrame->resizeView((Steinberg::IPlugView*)this, &m_rect);
			}
			*/
			//
			attachedToParent();
			//
			return Steinberg::kResultOk;
		}
		return Steinberg::kResultFalse;
	}

	//------------------------------------------------------------------------
	Steinberg::tresult PLUGIN_API MacWKWebView::removed()
	{
        std::cout << "\nMacWKWebView::removed()\n";
		//
		removedFromParent();
		//
		return Steinberg::kResultOk;
	}

	//------------------------------------------------------------------------
	Steinberg::tresult PLUGIN_API MacWKWebView::getSize(Steinberg::ViewRect* size)
	{
        std::cout << "\nMacWKWebView::getSize(Steinberg::ViewRect* size)\n";
		if (m_rect)
		{
			size->left   = m_rect->left;
			size->right  = m_rect->right;
			size->top    = m_rect->top;
			size->bottom = m_rect->bottom;
		}
		//
		return Steinberg::kResultOk;
	}

	//------------------------------------------------------------------------
	Steinberg::tresult PLUGIN_API MacWKWebView::onSize(Steinberg::ViewRect* newSize)
	{
        std::cout << "\nMacWKWebView::onSize(Steinberg::ViewRect* newSize)\n";
		if (m_rect)
		{
			m_rect->left = newSize->left;
			m_rect->right = newSize->right;
			m_rect->top = newSize->top;
			m_rect->bottom = newSize->bottom;
		}
		//
        /*
         if(m_controllerviewDll)
         {
         NSLog(@"\nChecking DLL for resizeWindow(NSRect newSize) function...\n");
         m_dllResizeChildWindowFunction = (void (*) (RECT)) GetProcAddress(m_controllerviewDll, "resizeWindow");
         if (m_dllResizeChildWindowFunction)
         {
             NSLog(@"\nCalling DLL resizeWindow(RECT newSize) function\n");
             NSRect windowSize;
             windowSize.left   = newSize->left;
             windowSize.right  = newSize->right;
             windowSize.top    = newSize->top;
             windowSize.bottom = newSize->bottom;
             //
             m_dllResizeChildWindowFunction(windowSize);
         }
         }
         */
		//
		return Steinberg::kResultOk;
	}

	//------------------------------------------------------------------------
	Steinberg::tresult PLUGIN_API MacWKWebView::setFrame(IPlugFrame* frame)
	{
        std::cout << "\nMacWKWebView::setFrame(IPlugFrame* frame)\n";
		//
		m_plugFrame = frame;
		//
		return Steinberg::kResultOk;
	}

	//------------------------------------------------------------------------
	Steinberg::tresult PLUGIN_API MacWKWebView::canResize()
	{
        std::cout << "\nMacWKWebView::canResize()\n";
		return Steinberg::kResultTrue;
	}

	//------------------------------------------------------------------------
	Steinberg::tresult PLUGIN_API MacWKWebView::checkSizeConstraint(Steinberg::ViewRect* rect)
	{
        std::cout << "\nMacWKWebView::checkSizeConstraint(Steinberg::ViewRect* rect)\n";
		if ( ((rect->right-rect->left) > 100)
				&&
			  ((rect->bottom-rect->top) > 100)
		   )
		{
			return Steinberg::kResultTrue;
		}
		else
		{
			return Steinberg::kResultFalse;
		}
	}

	//------------------------------------------------------------------------
	//------------------------------------------------------------------------
	Steinberg::tresult PLUGIN_API MacWKWebView::resizeView(Steinberg::IPlugView* view, Steinberg::ViewRect* newSize)
	{
        std::cout << "\nMacWKWebView::resizeView(Steinberg::IPlugView* view, Steinberg::ViewRect* newSize)\n";
		view = (Steinberg::IPlugView*)this;
		//
		if (m_rect)
		{
			newSize->left   = m_rect->left;
			newSize->right  = m_rect->right;
			newSize->top    = m_rect->bottom;
			newSize->bottom = m_rect->bottom;
		}
		//
		return Steinberg::kResultTrue;
	}

	//------------------------------------------------------------------------
	//------------------------------------------------------------------------
	/*
	HWND Win32WebView2::getChildWindow()
	{
		OutputDebugStringW(L"\nWin32WebView2::getChildWindow()\n");
		return m_windows->childWindow; // TODO
	}
	*/

} // namespace Carlsound
