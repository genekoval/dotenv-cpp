#include "parse.hpp"

#include <dotenv/dotenv>

#include <gtest/gtest.h>

using namespace dotenv::detail;
using namespace std::literals;

TEST(Trim, Empty) {
    EXPECT_EQ(""sv, trim(""));
}

TEST(Trim, Whitespace) {
    EXPECT_EQ(""sv, trim("   "));
}

TEST(Trim, NoSpaces) {
    EXPECT_EQ("foo"sv, trim("foo"));
}

TEST(Trim, Leading) {
    EXPECT_EQ("foo"sv, trim(" foo"));
}

TEST(Trim, Trailing) {
    EXPECT_EQ("foo"sv, trim("foo "));
}

TEST(Trim, LeadingTrailing) {
    EXPECT_EQ("foo"sv, trim("   foo   "));
}

TEST(Split, Simple) {
    const auto [name, value] = split("foo=bar");

    EXPECT_EQ("foo"sv, name);
    EXPECT_EQ("bar"sv, value);
}

TEST(Split, InlineComment) {
    const auto [name, value] = split("foo=bar # This is a comment");

    EXPECT_EQ("foo"sv, name);
    EXPECT_EQ("bar"sv, value);
}

TEST(Split, IncludeCommentCharacterNoSpaces) {
    const auto [name, value] = split("foo=bar#baz");

    EXPECT_EQ("foo"sv, name);
    EXPECT_EQ("bar#baz"sv, value);
}

TEST(Split, SingleQuotes) {
    const auto [name, value] = split("foo='bar'");

    EXPECT_EQ("foo"sv, name);
    EXPECT_EQ("bar"sv, value);
}

TEST(Split, DoubleQuotes) {
    const auto [name, value] = split(R"(foo="bar")");

    EXPECT_EQ("foo"sv, name);
    EXPECT_EQ("bar"sv, value);
}

TEST(Split, CommentCharacterQuoted) {
    const auto [name, value] = split("foo='bar # baz'");

    EXPECT_EQ("foo"sv, name);
    EXPECT_EQ("bar # baz"sv, value);
}

class DotenvTest : public testing::Test {
protected:
    static constexpr auto var1 = "DOTENV_ONE";
    static constexpr auto var2 = "DOTENV_TWO";
    static constexpr auto var3 = "DOTENV_THREE";

    static constexpr auto vars = std::array { var1, var2, var3 };

    DotenvTest() {
        for (const auto* var : vars) unset(var);
    }

    auto expect(const char* name, std::string_view value) {
        const auto* actual = std::getenv(name);

        EXPECT_TRUE(actual);
        if (actual) EXPECT_EQ(value, std::string_view(actual));
    }
};

class Parse : public DotenvTest {
protected:
    auto parse(const char* text) -> void {
        auto stream = std::istringstream(text);
        dotenv::detail::parse(stream);
    }
};

TEST_F(Parse, SingleAssignment) {
    parse("DOTENV_ONE=foo");
    expect(var1, "foo");
}

TEST_F(Parse, MultipleAssignments) {
    parse(
        "DOTENV_ONE=foo\n"
        "DOTENV_TWO=bar\n"
        "DOTENV_THREE=baz\n"
    );

    expect(var1, "foo");
    expect(var2, "bar");
    expect(var3, "baz");
}

TEST_F(Parse, EmptyLine) {
    parse(
        "DOTENV_ONE=foo\n"
        "\n"
        "DOTENV_TWO=bar\n"
    );

    expect(var1, "foo");
    expect(var2, "bar");
}

TEST_F(Parse, SkipCommentLine) {
    parse(
        "# This is a comment\n"
        "DOTENV_ONE=foo"
    );

    expect(var1, "foo");
}

TEST_F(Parse, Unset) {
    set(var1, "foo");
    expect(var1, "foo");

    parse("DOTENV_ONE=");
    EXPECT_FALSE(std::getenv(var1));
}

TEST_F(Parse, EmptySetFails) {
    EXPECT_THROW(parse("=foo"), dotenv::error);
}

TEST_F(Parse, EmptyUnsetFails) {
    EXPECT_THROW(parse("="), dotenv::error);
}

using Load = DotenvTest;

TEST_F(Load, Env) {
    dotenv::load("test/env");

    expect(var1, "foo");
    expect(var2, "contains # comment char");
    expect(var3, "bar#baz");
}
