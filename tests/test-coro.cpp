#include <comics/coro.h>

#include <gtest/gtest.h>

#include <string_view>

constexpr std::string_view ISSUES{R"(
    [
        {
            "id": "1",
            "issue number": "[nn]",
            "issue page count": "96.000",
            "issue page count uncertain": false,
            "key date": "1867-00-00",
            "language code": "en",
            "no volume": true,
            "price": "[none]",
            "publication date": "1867",
            "publisher country code": "us",
            "publisher name": "Roberts Brothers, Boston",
            "series country code": "us",
            "series name": "Two Hundred Sketches Humorous and Grotesque"
        }
    ])"};
constexpr std::string_view SEQUENCES{R"(
    [
        {
            "inks": "Gustave Dore",
            "issue": "1",
            "letters": "typeset",
            "pencils": "Gustave Dore",
            "sequence_number": "0",
            "title": "Two Hundred Sketches, Humorous and Grotesque",
            "title by gcd": false,
            "type": "cover"
        },
        {
            "inks": "Gustave Dore",
            "issue": "1",
            "pencils": "Gustave Dore",
            "sequence_number": "1",
            "title by gcd": false,
            "type": "illustration"
        }
    ])"};

TEST(TestComicsCoroutine, construct)
{
    const comics::Coroutine coro(ISSUES, SEQUENCES);
}
