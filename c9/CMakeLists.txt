add_executable(GeoServerTest
    LocationTest.cpp
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
)
