cmake_minimum_required(VERSION 3.15)
project(Seeit_Relay
  LANGUAGES CXX
  VERSION 1.0
)


include(FetchContent)

FetchContent_Declare(
  serial
  GIT_REPOSITORY "https://github.com/wjwwood/serial.git"
  GIT_TAG "1.2.1"
)

FetchContent_MakeAvailable(serial)
FetchContent_GetProperties(serial SOURCE_DIR mylibrary_SOURCE_DIR)


file(GLOB_RECURSE SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/serialrelay.cpp)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC 
                          ${CMAKE_CURRENT_SOURCE_DIR}/include
                          ${serial_SOURCE_DIR}/include)                       

target_link_libraries(${PROJECT_NAME} serial)