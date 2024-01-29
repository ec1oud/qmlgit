
find_package(Qt6Test)

add_executable(test gitcache/tst_gitcache.cpp)

target_link_libraries(test PRIVATE ${Qt6Quick_LIBRARIES} ${Qt6Test_LIBRARIES} ${QGit2_LIBRARIES} PkgConfig::LIBGIT2)

message(${CMAKE_BINARY_DIR})

include_directories(${CMAKE_BINARY_DIR}/tests)
