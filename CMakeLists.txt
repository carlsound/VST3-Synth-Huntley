set(PLUGIN_NAME "Huntley")
#
project(smtg-vst3-synth-${PLUGIN_NAME}
    VERSION ${vstsdk_VERSION}.0
    DESCRIPTION "Huntley synth virtual instrument"
)
#
set(BUNDLE_ID net.carlsound.${PLUGIN_NAME}.synth)
set(BUNDLE_IDENT "${BUNDLE_ID}")
#
set(CONTROLLER_VIEW ${PLUGIN_NAME}_ControllerView)
add_subdirectory(ControllerView)
#
add_subdirectory(Plugin)