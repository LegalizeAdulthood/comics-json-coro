#include <comics/comics.h>

#include <gtest/gtest.h>

#include <string_view>

TEST(TestComicsCoroutine, construct)
{
    std::string_view issues;
    std::string_view sequences;

    const comics::Coroutine coro(issues, sequences);
}
