
set(synth_sources
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
)

#--- HERE change the target Name for your plug-in (for ex. set(target myDelay))-------
if(MAC)
	set(target Huntley)
    smtg_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist" PREPROCESS)
elseif((WIN) AND (CMAKE_SIZEOF_VOID_P EQUAL "8"))
    set(target Huntley-x64)
elseif(WIN)
	set(target Huntley-x86)
endif()

smtg_add_vst3plugin(${target} ${SDK_ROOT} ${synth_sources})
set_target_properties(${target} PROPERTIES ${SDK_IDE_MYPLUGINS_FOLDER})
target_link_libraries(${target} PRIVATE base sdk)

if(WIN)
    target_sources(${target} PRIVATE resource/plug.rc)
endif()
