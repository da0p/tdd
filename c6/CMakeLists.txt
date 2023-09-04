add_executable(PortfolioTest
    PortfolioTest.cpp
)

target_link_libraries(PortfolioTest
    GTest::gtest
    GTest::gmock
    GTest::gtest_main
    Portfolio
)

add_library(Portfolio
    Portfolio.h
    Portfolio.cpp
)

target_link_libraries(Portfolio
PUBLIC
    boost::boost
)