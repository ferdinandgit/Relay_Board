cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(Seeit_Relay
  LANGUAGES CXX
  VERSION 1.0
)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(wxBUILD_SHARED OFF)

message(STATUS "Fetching wxWidgets...")

FetchContent_Declare(
   wxWidgets
   GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
   GIT_SHALLOW ON
)
FetchContent_MakeAvailable(wxWidgets)

message(STATUS "Fetching yaml cpp")

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG 0.8.0 # Can be a tag (yaml-cpp-x.x.x), a commit hash, or a branch name (master)
)
FetchContent_GetProperties(yaml-cpp)

if(NOT yaml-cpp_POPULATED)
  FetchContent_Populate(yaml-cpp)
  add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
endif()

file(COPY  ${CMAKE_CURRENT_SOURCE_DIR}/ressources DESTINATION  ${CMAKE_CURRENT_SOURCE_DIR}/build)

add_library(serial ${CMAKE_CURRENT_SOURCE_DIR}/src/serialib.cpp)
target_include_directories(serial PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


file(GLOB_RECURSE SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/src/mainframe.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/relayapp.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/controlpanel.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/drawingcanva.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/serialrelay.cpp
            )


if(APPLE)
    # create bundle on apple compiles
    add_executable(main MACOSX_BUNDLE ${SOURCES})

    # Set a custom plist file for the app bundle - needed for Mac OS Retina display
    set_target_properties(main PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
else()
    # the WIN32 is needed for Windows in order for it to look for WinMain
    # instead of the main function. This is ignored on other systems,
    # so it works on all platforms
    add_executable(main WIN32 ${SOURCES} main.exe.manifest)
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "Detected 64-bit architecture")
    set(HIDRELAY_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib/Win64/usb_relay_device.lib)
    set(HIDRELAY_DLL_PATH ${CMAKE_SOURCE_DIR}/lib/Win64/USB_RELAY_DEVICE.dll)
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(STATUS "Detected 32-bit architecture")
    set(HIDRELAY_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib/Win32/usb_relay_device.lib)
    set(HIDRELAY_DLL_PATH ${CMAKE_SOURCE_DIR}/lib/Win32/USB_RELAY_DEVICE.dll)
endif()

target_include_directories(main PUBLIC 
                          ${CMAKE_CURRENT_SOURCE_DIR}/include
                          )                       


if(WIN32)
    target_link_libraries(main PRIVATE wxcore wxnet serial ${HIDRELAY_LIBRARY_PATH})
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${HIDRELAY_DLL_PATH}"
        $<TARGET_FILE_DIR:main>)
else()
    target_link_libraries(main PRIVATE wxcore wxnet serial)
endif()

add_executable(inter ${CMAKE_CURRENT_SOURCE_DIR}/src/interpreter.cpp)
target_link_libraries(inter PUBLIC yaml-cpp::yaml-cpp)
target_include_directories(inter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(inter PRIVATE wxcore wxnet serial ${HIDRELAY_LIBRARY_PATH})
    add_custom_command(TARGET inter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/test.yaml"
        $<TARGET_FILE_DIR:inter>)