
include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Quick_INCLUDE_DIRS}
    ${QGit2_INCLUDE_DIRS}
    ${Git2_INCLUDE_DIRS}
)

set(qmlgit_SRCS
    beziercurve.cpp
    diffhighlighter.cpp
    gitcache.cpp
    gitcommitlist.cpp
#     gitdiff.cpp
    gitrepo.cpp
    main.cpp
)

qt5_add_resources(RES ../resources.qrc)
add_executable(qmlgit ${qmlgit_SRCS} ${RES})
target_link_libraries(qmlgit ${Qt5Quick_LIBRARIES} ${LibQGit2_LIBRARIES})
