# https://cmake.org/cmake/help/latest/prop_tgt/FRAMEWORK.html

enable_language(OBJCXX)

set(TARGET_FILES
    #./include/dllmain.h
    #./source/dllmain.cpp
    #./library_exports.h
    #./library_exports.c
    #
    #./include/Win32ChildHwnd.h
    #./source/Win32ChildHwnd.cpp
    include/Cocoa_UI_Interface.h
    source/Cocoa_UI_Interface.mm
    #
    #./resource/WKWebView_Test_UI.html
    #./resource/WKWebView_Test_UI.js
    #
    #./resource/EditorWindowController.xib
    #./resource/Info.plist
	)

  set(RESOURCE_FILE_1 "./resource/WKWebView_Test_UI.html")
  set(RESOURCE_FILE_2 "./resource/WKWebView_Test_UI.js")
  set(RESOURCE_FILES 
      "./resource/WKWebView_Test_UI.html"
      "./resource/WKWebView_Test_UI.js"
      )

add_library(${CONTROLLER_VIEW} SHARED ${TARGET_FILES} 
            ${RESOURCE_FILE_1}
            ${RESOURCE_FILE_2}
            )

#find_library(COCOA_FRAMEWORK Cocoa)
    #if (NOT COCOA_FRAMEWORK)
        #message(FATAL_ERROR "Cocoa.framework not found")
    #endif()
target_link_libraries(${CONTROLLER_VIEW} PRIVATE base sdk objc
    #${COCOA_FRAMEWORK}     
    PUBLIC "-framework Cocoa"
    PUBLIC "-framework WebKit"
    )

set_target_properties(${CONTROLLER_VIEW} PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION "A"
  #
  MACOSX_FRAMEWORK_IDENTIFIER ${BUNDLE_ID}
  #MACOSX_FRAMEWORK_INFO_PLIST ./resource/Info.plist
  # "current version" in semantic format in Mach-O binary file
  VERSION 16.4.0
  # "compatibility version" in semantic format in Mach-O binary file
  SOVERSION 1.0.0
  #
  PUBLIC_HEADER "Cocoa_UI_Interface.h"
  #
  #RESOURCE ${RESOURCE_FILES}
  RESOURCE "${RESOURCE_FILE_1};${RESOURCE_FILE_2}"
  #RESOURCE ${RESOURCE_FILE_2}
  #
  #XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Apple Development"
  #XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "John Carlson (Personal Team)"
  XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ${DEVELOPMENT_TEAM_ID}
  #XCODE_LINK_BUILD_PHASE_MODE KNOWN_LOCATION #BUILT_ONLY
  #XCODE_EMBED_FRAMEWORKS "Cocoa"
  XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_WEAK YES
)
#
#set_source_files_properties(${RESOURCE_FILE_1} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
#set_source_files_properties(${RESOURCE_FILE_2} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)