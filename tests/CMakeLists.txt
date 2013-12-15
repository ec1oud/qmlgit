
find_package(Qt5Test)

add_executable(test gitcache/tst_gitcache.cpp)

target_link_libraries(test ${Qt5Quick_LIBRARIES} ${Qt5Test_LIBRARIES} ${LibQGit2_LIBRARIES})

message(${CMAKE_BINARY_DIR})

include_directories(${CMAKE_BINARY_DIR}/tests)
