#include <catch2/catch.hpp>
#include <debugger_trace.hpp>

namespace eureka
{
    struct NeitherStreamableNorFormattable
    {
        int val;
    };
    struct StreamableNotFormattable
    {
        int val;
    };

    std::ostream& operator<<(std::ostream& os, const StreamableNotFormattable& obj)
    {
        return os << "StreamableNotFormattable{" << obj.val << "}";
    }

    static_assert(streamable<StreamableNotFormattable>);
    static_assert(streamable<StreamableNotFormattable&>);
    static_assert(streamable<std::string>);
    static_assert(streamable<std::string>);
    static_assert(streamable<std::string&>);
    static_assert(streamable<const std::string&>);
    static_assert(streamable<int>);
    static_assert(streamable<int&>);
    static_assert(!streamable<std::vector<int>>);
    static_assert(!streamable<std::vector<int>&>);
    static_assert(iterable<std::vector<int>>);
    static_assert(iterable<std::vector<int>&>);
    static_assert(iterable<std::string>);

}

TEST_CASE("formating string", "[formatting]")
{
    auto result = std::format("{}", std::string("hi"));

    CHECK(result == "hi");
}

TEST_CASE("formatting streamable that is not formattable", "[formatting]")
{
    auto result = std::format("{}", eureka::StreamableNotFormattable{ 123 });

    CHECK(result == "StreamableNotFormattable{123}");
}

TEST_CASE("formatting iterable of formattable", "[formatting]")
{
    std::vector<int> vals{ 1,2,3 };
    auto result = std::format("{}", vals);

    CHECK(result == "{1,2,3}");
}

TEST_CASE("formatting iterable of streamable that is not formattable", "[formatting]")
{
    std::vector<eureka::StreamableNotFormattable> vals{ {1},{2} };
    auto result = std::format("{}", vals);

    CHECK(result == "{StreamableNotFormattable{1},StreamableNotFormattable{2}}");
}
