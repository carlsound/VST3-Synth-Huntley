#--- HERE change the target Name for your plug-in (for ex. set(target myDelay))-------
if(APPLE)
	set(target Huntley)
elseif((WIN32) AND (CMAKE_SIZEOF_VOID_P EQUAL "8"))
    set(target Huntley_x64)
elseif(WIN32)
	set(target Huntley_x86)
endif()

smtg_add_vst3plugin(${target}
#set(synth_sources
    include/noteExpressionSynthController.h
    source/noteExpressionSynthController.cpp
    #---
    include/noteExpressionSynthIDs.h
    #---
    include/noteExpressionSynthProcessor.h
    source/noteExpressionSynthProcessor.cpp
    #---
    include/version.h
    #---
    source/noteExpressionSynthFactory.cpp
    #---
    include/noteExpressionSynthVoice.h
    source/noteExpressionSynthVoice.cpp
    #---
    include/panNoteExpressionType.h
    #---
    include/releaseTimeModNoteExpressionType.h
    #---
    include/brownnoise.h
    #---
    include/filter.h
    #---
    #---https://github.com/micknoise/Maximilian
    #---include/maximilian.h
    #---include/maximilian.cpp
    #---
    include/MacWKWebView.h
    source/MacWKWebView.cpp
    #---
    include/IPlugViewLifetimeEventHandler.h
)

if(APPLE)
    #smtg_target_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist" PREPROCESS)
    smtg_target_set_bundle(${target}
        BUNDLE_IDENTIFIER ${BUNDLE_ID}
        COMPANY_NAME "Carlsound"
    )
    #
    set_target_properties(${target} PROPERTIES
            XCODE_LINK_BUILD_PHASE_MODE BUILT_ONLY #KNOWN_LOCATION        
            XCODE_EMBED_FRAMEWORKS "${CONTROLLER_VIEW}"
            XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY ON
    #        
    #        MACOSX_BUNDLE TRUE
    #    MACOSX_FRAMEWORK_IDENTIFIER
    #        "net.carlsound.01.Test.NSView.Mac"
    #    RESOURCES
    #        "../resource/EditorViewController.xib"
    #        "../resource/EditorWindowController.xib"
            #XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Apple Development"
            #XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "John Carlson (Personal Team)"
            XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ${DEVELOPMENT_TEAM_ID}
    )
    #
    configure_file(../bundleID.h.in 
    ${PROJECT_BINARY_DIR}/include/bundleID.h 
    @ONLY)
    #
    target_include_directories(${target} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    "${PROJECT_BINARY_DIR}/include")
    #
elseif(WIN32)
    target_sources(${target} PRIVATE resource/plug.rc)
    #
    target_include_directories(${target} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()

set_target_properties(${target} PROPERTIES
    # ${SDK_IDE_MYPLUGINS_FOLDER}
    FOLDER ${PLUGIN_NAME}
    CXX_STANDARD 17)
target_link_libraries(${target} PRIVATE base sdk)