include(GoogleTest)

find_package(GTest CONFIG REQUIRED)

add_executable(test-comics-json-coro
    test-coro.cpp
)
target_link_libraries(test-comics-json-coro comics GTest::gtest_main)
set_target_properties(test-comics-json-coro PROPERTIES FOLDER "Tests")

gtest_discover_tests(test-comics-json-coro)
