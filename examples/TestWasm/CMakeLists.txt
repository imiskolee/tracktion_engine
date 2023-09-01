cmake_minimum_required(VERSION 3.15)

set (TARGET_NAME TestWasm)

if (DEFINED PROJECT_NAME)
    message ("Adding ${TARGET_NAME} to the ${PROJECT_NAME} project")
else()
    file(READ ../../VERSION.md CURRENT_VERSION)
    project(${TARGET_NAME} VERSION ${CURRENT_VERSION})
    add_subdirectory(../../modules/juce ./cmake_build_juce)
    add_subdirectory(../../modules ./cmake_build_tracktion)
endif()


set(CMAKE_EXECUTABLE_SUFFIX ".html")

# Adds all the module sources so they appear correctly in the IDE
set_property(GLOBAL PROPERTY USE_FOLDERS YES)
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -sAUDIO_WORKLET -sWASM_WORKERS -sUSE_PTHREADS=1")
juce_add_console_app(${TARGET_NAME})

juce_generate_juce_header(${TARGET_NAME})

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)

set_target_properties(${TARGET_NAME} PROPERTIES
    C_VISIBILITY_PRESET hidden
    CXX_VISIBILITY_PRESET hidden)
set(SourceFiles
        TestWasm.h
        TestWasm.cpp)

target_sources(${TARGET_NAME} PRIVATE ${SourceFiles})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX Source FILES ${SourceFiles})


target_compile_definitions(${TARGET_NAME} PRIVATE
    JUCE_PLUGINHOST_AU=0
    JUCE_PLUGINHOST_LADSPA=0
    JUCE_PLUGINHOST_VST3=0
    JUCE_USE_CURL=0
    JUCE_WEB_BROWSER=0
    JUCER_ENABLE_GPL_MODE=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_REPORT_APP_USAGE=0
    JUCE_STRICT_REFCOUNTEDPOINTER=1
    JUCE_MODAL_LOOPS_PERMITTED=1
    TRACKTION_ENABLE_TIMESTRETCH_SOUNDTOUCH=1
    TRACKTION_UNIT_TESTS=0
    TRACKTION_LOG_DEVICES=0)

target_link_libraries(${TARGET_NAME} PRIVATE
    tracktion::tracktion_core
    tracktion::tracktion_engine
    tracktion::tracktion_graph
    juce::juce_audio_devices
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_recommended_warning_flags)


