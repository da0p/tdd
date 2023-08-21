add_executable(SoundexTest
    SoundexTest.cpp
)

target_link_libraries(SoundexTest
    GTest::gtest
    GTest::gtest_main
)