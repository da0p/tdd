add_executable(GeoServerTest
    LocationTest.cpp
    AreaTest.cpp
    WorkTest.cpp
    GeoServerTest.cpp
)

target_link_libraries(GeoServerTest
    GTest::gtest
    GTest::gmock
    GTest::gtest_main
    GeoServer
)

add_library(GeoServer
    Location.h
    Location.cpp
    Area.h
    Area.cpp
    GeoServer.h
    GeoServer.cpp
    Work.h
)
