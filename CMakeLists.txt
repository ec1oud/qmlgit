cmake_minimum_required(VERSION 2.8)
project(qmlgit)

find_package(Qt5Core)
find_package(Qt5Quick)
find_package(Qt5Widgets)

find_package(LibQGit2)

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
add_definitions(${Qt5Core_DEFINITIONS} ${Qt5Quick_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTOMOC TRUE)

add_subdirectory(src)
add_subdirectory(tests)
