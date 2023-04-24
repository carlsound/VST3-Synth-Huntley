//------------------------------------------------------------------------
// Copyright(c) 2021 Carlsound.
//------------------------------------------------------------------------
//
#pragma once
//
#include "public.sdk/source/common/pluginview.h"
#include "base/source/fobject.h"
#include "pluginterfaces/base/ftypes.h"
#include "pluginterfaces/gui/iplugview.h"
//
#include "IPlugViewLifetimeEventHandler.h"
//#include "../ControllerView/Win32ChildHwnd.h"
//
#include <memory>
//
//------------------------------------------------------------------------
//extern HINSTANCE ghInst;
//extern bool InitDll();
//
//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
//
namespace Carlsound
{
	//------------------------------------------------------------------------
	//  WebView2TestController
	//------------------------------------------------------------------------
	class MacWKWebView : public Steinberg::CPluginView,
						  //public Steinberg::IEditorFactory,
						  public Steinberg::IPlugFrame
	{
	public:
	//------------------------------------------------------------------------
		MacWKWebView(); // = default;
		~MacWKWebView(); // SMTG_OVERRIDE = default;

		void closeWindow();

		// CPluginView
		void attachedToParent() override;
		void removedFromParent() override;

		// IPluginView
		Steinberg::tresult PLUGIN_API isPlatformTypeSupported(Steinberg::FIDString type) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API attached(void* parent, Steinberg::FIDString type) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API removed() SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API getSize(Steinberg::ViewRect* size) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API onSize(Steinberg::ViewRect* newSize) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API setFrame(IPlugFrame* frame) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API canResize() SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API checkSizeConstraint(Steinberg::ViewRect* rect) SMTG_OVERRIDE;

		// IPlugFrame
		Steinberg::tresult PLUGIN_API resizeView(Steinberg::IPlugView* view, Steinberg::ViewRect* newSize) override;

		//
		//HWND getChildWindow();

		//---Interfaces---------
		OBJ_METHODS(MacWKWebView, CPluginView)
		DEFINE_INTERFACES
			DEF_INTERFACE(IPlugFrame)
		END_DEFINE_INTERFACES(CPluginView)
		DELEGATE_REFCOUNT(CPluginView)
		//------------------------------------------------------------------------
	protected:
		//------------------------------------------------------------------------
		
		//
	}; // class Win32WebView2

	//------------------------------------------------------------------------
} // namespace Carlsound


//------------------------------------------------------------------------
//DWORD WINAPI UI_Thread(LPVOID lpParam);
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
