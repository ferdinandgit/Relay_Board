cmake_minimum_required(VERSION 3.15)
project(Seeit_Relay
  LANGUAGES CXX
  VERSION 1.0
)

add_library(serial ${CMAKE_CURRENT_SOURCE_DIR}/src/serialib.cpp)
target_include_directories(serial PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


file(GLOB_RECURSE SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/serialrelay.cpp)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC 
                          ${CMAKE_CURRENT_SOURCE_DIR}/include
                          )                       

target_link_libraries(${PROJECT_NAME} serial)