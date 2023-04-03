project(smtg-vst3-Huntley
    VERSION ${vstsdk_VERSION}.0
    DESCRIPTION "virtual instrument"
)
#
set(CONTROLLER_VIEW "Huntley_ControllerView")
set(BUNDLE_ID "net.carlsound.Huntley")
set(BUNDLE_IDENT "${BUNDLE_ID}")
#
add_subdirectory(ControllerView)
add_subdirectory(Plugin)