#include <comics/coro.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <ostream>
#include <string_view>

using namespace testing;

constexpr std::string_view SCRIPT_NAME{"Stan Lee"};
constexpr std::string_view PENCIL_NAME{"Jack Kirby"};
constexpr std::string_view INK_NAME{"Steve Ditko"};
constexpr std::string_view ISSUES{R"ish(
    [
        {
            "brand group names": "Marvel",
            "brand name": "MC",
            "display number": "#1",
            "id": "16556",
            "indicia publisher name": "Canam Publishers Sales Corp.",
            "issue number": "1",
            "issue page count": "36.000",
            "issue page count uncertain": false,
            "key date": "1961-11-00",
            "language code": "en",
            "no volume": false,
            "price": "0.10 USD",
            "publication date": "November 1961",
            "publisher country code": "us",
            "publisher name": "Marvel",
            "series country code": "us",
            "series name": "Fantastic Four",
            "volume": "1"
        },
        {
            "brand group names": "Marvel",
            "brand name": "MC",
            "display number": "#1",
            "id": "17568",
            "indicia publisher name": "Non-Pareil Publishing Corp.",
            "issue number": "1",
            "issue page count": "36.000",
            "issue page count uncertain": false,
            "key date": "1963-03-00",
            "language code": "en",
            "no volume": false,
            "price": "0.12 USD",
            "publication date": "March 1963",
            "publisher country code": "us",
            "publisher name": "Marvel",
            "series country code": "us",
            "series name": "The Amazing Spider-Man",
            "volume": "1"
        }
    ])ish"};
constexpr std::string_view SEQUENCES{R"seq(
    [
        {
            "characters": "The Fantastic Four [The Invisible Girl [Sue Storm]; The Human Torch [Johnny Storm]; The Thing [Ben Grimm]; Mr. Fantastic [Reed Richards]]; Giganto",
            "colors": "Stan Goldberg (see notes)",
            "feature": "Fantastic Four",
            "genre": "superhero",
            "inks": "George Klein (see notes)",
            "issue": "16556",
            "letters": "Artie Simek (cover lettering and logo design); Sol Brodsky (logo design, see notes)",
            "pencils": "Jack Kirby",
            "script": "Stan Lee",
            "sequence_number": "0",
            "title by gcd": false,
            "type": "cover"
        },
        {
            "characters": "The Fantastic Four [Mr. Fantastic [Reed Richards]; The Thing [Ben Grimm]; Invisible Girl [Sue Storm]; Human Torch [Johnny Storm]] (introduction, origin); the Mayor of Central City; Central City Police Department [Pete; unnamed members]; National Guard unnamed members",
            "colors": "Stan Goldberg",
            "feature": "Fantastic Four",
            "genre": "superhero",
            "inks": "George Klein; Sol Brodsky ? (see notes)",
            "issue": "16556",
            "letters": "Artie Simek",
            "pencils": "Jack Kirby (signed as Jack Kirby [signature])",
            "script": "Stan Lee (signed as Stan Lee [early- to mid-career])",
            "sequence_number": "2",
            "title": "The Fantastic Four!",
            "title by gcd": false,
            "type": "comic story"
        },
        {
            "characters": "The Fantastic Four [Mr. Fantastic [Reed Richards]; The Human Torch [Johnny Storm]; The Invisible Girl [Sue Storm]; The Thing [Ben Grimm]]; Mole Man [Moleman] (introduction); Monsters of Monster Isle [Giganto; Tricephalous] (introduction, all unnamed here); French Army [Pierre; unnamed soldiers]",
            "colors": "Stan Goldberg",
            "feature": "Fantastic Four",
            "genre": "superhero",
            "inks": "George Klein; Sol Brodsky ? (see notes)",
            "issue": "16556",
            "letters": "Artie Simek",
            "pencils": "Jack Kirby",
            "script": "Stan Lee",
            "sequence_number": "5",
            "title": "The Fantastic Four Meet the Mole Man!",
            "title by gcd": false,
            "type": "comic story"
        },
        {
            "characters": "Ben Grimm; Human Torch; Invisible Girl; Mister Fantastic; Peter Parker; Reed Richards; Spider-Man; Johnny Storm; Sue Storm; The Thing",
            "colors": "Stan Goldberg",
            "feature": "Spider-Man",
            "genre": "superhero",
            "inks": "Steve Ditko",
            "issue": "17568",
            "letters": "Artie Simek (cover lettering and logo inking); Sol Brodsky (logo design)",
            "pencils": "Jack Kirby",
            "script": "Stan Lee",
            "sequence_number": "0",
            "title": "The Chameleon Strikes!",
            "title by gcd": false,
            "type": "cover"
        },
        {
            "characters": "Liz Allan; J. Jonah Jameson (introduction); John Jameson (introduction); May Parker; Peter Parker; Spider-Man; Flash Thompson; The Burglar (flashback)",
            "colors": "Stan Goldberg",
            "feature": "Spider-Man",
            "genre": "superhero",
            "inks": "Steve Ditko (credited) (art)",
            "issue": "17568",
            "letters": "Jon D&#x27;Agostino (credited as  Johnny Dee) (lettering)",
            "pencils": "Steve Ditko (credited) (art)",
            "script": "Stan Lee (credited)",
            "sequence_number": "1",
            "title": "Spider-Man",
            "title by gcd": false,
            "type": "comic story"
        },
        {
            "characters": "Chameleon (antagonist) (introduction); Ben Grimm; Human Torch; Invisible Girl; Mister Fantastic; Peter Parker; Reed Richards; Dmitri Smerdyakov (antagonist) (introduction); Spider-Man; Johnny Storm; Sue Storm; The Thing",
            "colors": "Stan Goldberg",
            "feature": "Spider-Man",
            "genre": "superhero",
            "inks": "Steve Ditko (art)",
            "issue": "17568",
            "letters": "John Duffy (credited as  John Duffi)",
            "pencils": "Steve Ditko (credited) (art)",
            "script": "Stan Lee (credited)",
            "sequence_number": "2",
            "title": "Spider-Man vs. The Chameleon!",
            "title by gcd": false,
            "type": "comic story"
        }
    ])seq"};

class MockDatabase : public testing::StrictMock<comics::Database>
{
public:
    ~MockDatabase() override = default;

    MOCK_METHOD(const simdjson::simdjson_result<simdjson::dom::element> &, getIssues, (), (const, override));
    MOCK_METHOD(const simdjson::simdjson_result<simdjson::dom::element> &, getSequences, (), (const, override));
};

struct ParsedJson
{
    explicit ParsedJson(std::string_view json);
    simdjson::dom::parser m_parser;
    simdjson::simdjson_result<simdjson::dom::element> m_document;
};

ParsedJson::ParsedJson(std::string_view json) :
    m_document(m_parser.parse(json.data(), json.size()))
{
}

using MockDatabasePtr = std::shared_ptr<MockDatabase>;

inline MockDatabasePtr createMockDatabase()
{
    return std::make_shared<MockDatabase>();
}

TEST(TestComicsCoroutine, construct)
{
    const comics::Coroutine coro{nullptr, "script", SCRIPT_NAME};
}

TEST(TestComicsCoroutine, notResumableFromNoDatabase)
{
    const comics::Coroutine coro{nullptr, "script", SCRIPT_NAME};

    const bool resumable{coro.resume()};

    EXPECT_FALSE(resumable);
}

TEST(TestComicsCoroutine, notResumableFromNullDatabase)
{
    MockDatabasePtr db{createMockDatabase()};
    simdjson::simdjson_result<simdjson::dom::element> issues;
    simdjson::simdjson_result<simdjson::dom::element> sequences;
    EXPECT_CALL(*db, getIssues()).WillOnce(ReturnRef(issues));
    EXPECT_CALL(*db, getSequences()).WillOnce(ReturnRef(sequences));
    comics::Coroutine coro{db, "script", SCRIPT_NAME};

    const bool resumable{coro.resume()};

    ASSERT_FALSE(resumable);
}

TEST(TestComicsCoroutine, notResumableFromNullIssues)
{
    MockDatabasePtr db{createMockDatabase()};
    simdjson::simdjson_result<simdjson::dom::element> issues;
    ParsedJson sequences("[]");
    EXPECT_CALL(*db, getIssues()).WillOnce(ReturnRef(issues));
    EXPECT_CALL(*db, getSequences()).WillOnce(ReturnRef(sequences.m_document));
    comics::Coroutine coro{db, "script", SCRIPT_NAME};

    const bool resumable{coro.resume()};

    ASSERT_FALSE(resumable);
}

TEST(TestComicsCoroutine, notResumableFromNullSequences)
{
    MockDatabasePtr db{createMockDatabase()};
    ParsedJson issues("[]");
    simdjson::simdjson_result<simdjson::dom::element> sequences;
    EXPECT_CALL(*db, getIssues()).WillOnce(ReturnRef(issues.m_document));
    EXPECT_CALL(*db, getSequences()).WillOnce(ReturnRef(sequences));
    comics::Coroutine coro{db, "script", SCRIPT_NAME};

    const bool resumable{coro.resume()};

    ASSERT_FALSE(resumable);
}

TEST(TestComicsCoroutine, notResumableFromEmptyIssues)
{
    MockDatabasePtr db{createMockDatabase()};
    ParsedJson issues("[]");
    ParsedJson sequences("[{}]");
    EXPECT_CALL(*db, getIssues()).WillOnce(ReturnRef(issues.m_document));
    EXPECT_CALL(*db, getSequences()).WillOnce(ReturnRef(sequences.m_document));
    comics::Coroutine coro{db, "script", SCRIPT_NAME};

    const bool resumable{coro.resume()};

    ASSERT_FALSE(resumable);
}

TEST(TestComicsCoroutine, notResumableFromEmptySequences)
{
    MockDatabasePtr db{createMockDatabase()};
    ParsedJson issues("[{}]");
    ParsedJson sequences("[]");
    EXPECT_CALL(*db, getIssues()).WillOnce(ReturnRef(issues.m_document));
    EXPECT_CALL(*db, getSequences()).WillOnce(ReturnRef(sequences.m_document));
    comics::Coroutine coro{db, "script", SCRIPT_NAME};

    const bool resumable{coro.resume()};

    ASSERT_FALSE(resumable);
}

TEST(TestComicsCoroutine, resumableFromPopulatedArrays)
{
    MockDatabasePtr db{createMockDatabase()};
    ParsedJson issues(ISSUES);
    ParsedJson sequences(SEQUENCES);
    EXPECT_CALL(*db, getIssues()).WillOnce(ReturnRef(issues.m_document));
    EXPECT_CALL(*db, getSequences()).WillOnce(ReturnRef(sequences.m_document));
    comics::Coroutine coro{db, "script", "Slartibartfast"};

    const bool resumable{coro.resume()};

    ASSERT_TRUE(resumable);
}
