cmake_minimum_required(VERSION 3.16)
project(qmlgit)

find_package(Qt6Core)
find_package(Qt6Quick)
find_package(Qt6Widgets)

find_package(QGit2)
find_package(PkgConfig)
pkg_check_modules(LIBGIT2 "libgit2>=1.0" REQUIRED IMPORTED_TARGET)

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
add_definitions(${Qt6Core_DEFINITIONS} ${Qt6Quick_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${Qt6Core_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTOMOC TRUE)

add_subdirectory(src)
add_subdirectory(tests)
