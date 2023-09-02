add_executable(PlaceDescriptionServiceTest
    PlaceDescriptionServiceTest.cpp
)

target_link_libraries(PlaceDescriptionServiceTest
    GTest::gtest
    GTest::gtest_main
)