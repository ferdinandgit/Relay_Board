cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(DriveRelay
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
            ${CMAKE_CURRENT_SOURCE_DIR}/app.rc
            )


if(APPLE)
    add_executable(DriveRelay MACOSX_BUNDLE ${SOURCES})
    set_target_properties(DriveRelay PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
else()
    add_executable(DriveRelay WIN32 ${SOURCES} main.exe.manifest)
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

target_include_directories(DriveRelay PUBLIC 
                          ${CMAKE_CURRENT_SOURCE_DIR}/include
                          )                       


if(WIN32)
    target_link_libraries(DriveRelay PUBLIC wxcore wxnet serial ${HIDRELAY_LIBRARY_PATH} yaml-cpp::yaml-cpp)
    add_custom_command(TARGET DriveRelay POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${HIDRELAY_DLL_PATH}"
        $<TARGET_FILE_DIR:DriveRelay>)
else()
    target_link_libraries(DriveRelay PUBLIC wxcore wxnet serial hidusbrelay yaml-cpp::yaml-cpp)
endif()

# Installation rules
install(TARGETS DriveRelay DESTINATION bin)
install(FILES ${HIDRELAY_DLL_PATH} DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/ressources/icons/iconmanual.bmp DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/ressources/icons/logo.bmp DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/ressources/icons/prog.bmp DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/ressources/icons/test.bmp DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/ressources/icons/prog.ico DESTINATION bin)

# Include CPack for packaging

# Basic package information
set(CPACK_GENERATOR "NSIS")
set(CPACK_PACKAGE_NAME "DriveRelay")
set(CPACK_PACKAGE_VENDOR "SEEIT")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_CONTACT "contact@seeit.fr")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "DriveRelay Installer")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "DriveRelay")
set(CPACK_PACKAGE_EXECUTABLES "DriveRelay" "DriveRelay")

# Set the NSIS script file path
set(CPACK_NSIS_SCRIPT "${CMAKE_SOURCE_DIR}/MyInstaller.nsi")

# Set the installer icon
set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/ressources/icons/prog.ico")

# Include CPack settings
include(CPack)