
include_directories(
    ${Qt6Core_INCLUDE_DIRS}
    ${Qt6Quick_INCLUDE_DIRS}
    ${Qt6Widgets_INCLUDE_DIRS}
    ${QGit2_INCLUDE_DIRS}
    ${Git2_INCLUDE_DIRS}
)

set(qmlgit_SRCS
    beziercurve.cpp
    diffhighlighter.cpp
    gitcache.cpp
    gitdragmodel.cpp
    gitmodel.cpp
    gitrepo.cpp
    main.cpp
)

qt6_add_resources(RES ../resources.qrc)
add_executable(qmlgit ${qmlgit_SRCS} ${RES})
target_link_libraries(qmlgit PRIVATE ${Qt6Widgets_LIBRARIES} ${Qt6Quick_LIBRARIES} ${QGit2_LIBRARIES} PkgConfig::LIBGIT2)
