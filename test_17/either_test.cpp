#include <gtest/gtest.h>
#include <random>
#include <boost/range/irange.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <either.h>

TEST(EitherTest, simple) {
    Either<int, std::string> e_one(10);
    ASSERT_EQ(e_one.get_left(), 10);
    Either<int, std::string> e_two(std::string("*"));
    ASSERT_EQ(e_two.get_right(), std::string("*"));
    Either<int, std::string> e_three("*");
    ASSERT_EQ(e_three.get_right(), std::string("*"));
    ASSERT_EQ(e_three.get_right(), "*");
    Either<int, std::string> e_four = Either<int, std::string>("+");
    ASSERT_EQ(e_four.get_right(),"+");
    Either<int, std::string> e_five = std::move(e_four);
    ASSERT_EQ(e_five.get_right(), "+");
}

TEST(EitherTest, containers) {
    using EitherIntOrString = Either<int, std::string>;
    auto gen = boost::irange(1, 1000);
    std::vector<EitherIntOrString> ve;
    // std c++ way without ranges:
    //    std::transform(std::begin(gen),
    //                   std::end(gen),
    //                   std::back_inserter(ve),
    //                   [](int i) {return i % 2 ? EitherIntOrString(i): EitherIntOrString(std::to_string(i));});

    // with boost::ranges -> clearer intent
    ve = boost::copy_range<decltype(ve)>(gen | boost::adaptors::transformed([](int i) {return i % 2 ? EitherIntOrString(i): EitherIntOrString(std::to_string(i));}));
    boost::range::for_each(ve | boost::adaptors::indexed(1), [](const auto& item){
        if (item.index() % 2 ) {
            ASSERT_TRUE(item.value().has_left());
            ASSERT_FALSE(item.value().has_right());
        }
        else {
            ASSERT_FALSE(item.value().has_left());
            ASSERT_TRUE(item.value().has_right());
        }
        });
}

TEST(EitherTest, containers_ranges) {
    using EitherIntOrString = Either<int, std::string>;
    auto gen = boost::irange(1, 1000);

    // the test lambda
    auto check_odd_is_right = [](const auto& item){
        if (item.index() % 2 ) {
            ASSERT_TRUE(item.value().has_left());
            ASSERT_FALSE(item.value().has_right());
        }
        else {
            ASSERT_FALSE(item.value().has_left());
            ASSERT_TRUE(item.value().has_right());
        }
    };

    // generator lambda for the testdata
    auto odd_even_either = [](int i) {return i % 2 ? EitherIntOrString(i): EitherIntOrString(std::to_string(i));};

    // as pure range solution
    boost::range::for_each(gen | boost::adaptors::transformed(odd_even_either)
                                    | boost::adaptors::indexed(1),
                                    check_odd_is_right );
}
