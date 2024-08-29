cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

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
  GIT_TAG 0.8.0
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
            ${CMAKE_CURRENT_SOURCE_DIR}/src/interpreter.cpp
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

if(WIN32)
    message(STATUS "Windows")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        message(STATUS "Detected 64-bit architecture")
        set(HIDRELAY_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib/Win64/usb_relay_device.lib)
        set(HIDRELAY_DLL_PATH ${CMAKE_SOURCE_DIR}/lib/Win64/USB_RELAY_DEVICE.dll)
    elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
        message(STATUS "Detected 32-bit architecture")
        set(HIDRELAY_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib/Win32/usb_relay_device.lib)
        set(HIDRELAY_DLL_PATH ${CMAKE_SOURCE_DIR}/lib/Win32/USB_RELAY_DEVICE.dll)
    endif()
else()
    message(STATUS "Unix")
    add_library(hidusbrelay SHARED IMPORTED)
    set_target_properties( hidusbrelay PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/Unix/usb_relay_device.so )
endif()

target_include_directories(main PUBLIC 
                          ${CMAKE_CURRENT_SOURCE_DIR}/include
                          )                       


if(WIN32)
    target_link_libraries(main PUBLIC wxcore wxnet serial ${HIDRELAY_LIBRARY_PATH} yaml-cpp::yaml-cpp)
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${HIDRELAY_DLL_PATH}"
        $<TARGET_FILE_DIR:main>)
else()
    target_link_libraries(main PUBLIC wxcore wxnet serial hidusbrelay yaml-cpp::yaml-cpp)
endif()


