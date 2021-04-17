#include <gtest/gtest.h>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/irange.hpp>
#include <boost/timer/timer.hpp>
#include <random>
#include <execution>

TEST(BoostRangesTest, boost_one) {
    const int mx_val = 32'000'000;
    auto gen = boost::irange(12, mx_val);
    // boost auto_cpu_timer outputs the time spend in this scope
    {
        boost::timer::auto_cpu_timer t;
        std::vector<int> vi(std::begin(gen), std::end(gen));
        EXPECT_EQ(vi.size(), (mx_val - 12));
    }
}

TEST(BoostRangesTest, boost_sort_parallel) {
    const int mx_val = 32'000;
    auto gen = boost::irange(12, mx_val);
    std::random_device rd;
    std::mt19937 g(rd());
    std::vector<int> vi(std::begin(gen), std::end(gen));
    std::shuffle(std::begin(vi), std::end(vi), g);
    {
        boost::timer::auto_cpu_timer t;
        std::sort(std::execution::par, std::begin(vi), std::end(vi));
    }
    ASSERT_TRUE(std::is_sorted(std::execution::par, std::begin(vi), std::end(vi)));
}

TEST(BoostRangesTest, boost_sort_sequential) {
    const int mx_val = 32'000;
    auto gen = boost::irange(12, mx_val);
    std::random_device rd;
    std::mt19937 g(rd());
    std::vector<int> vi(std::begin(gen), std::end(gen));
    std::shuffle(std::begin(vi), std::end(vi), g);
    {
        boost::timer::auto_cpu_timer t;
        std::sort(std::execution::seq, std::begin(vi), std::end(vi));
    }
    ASSERT_TRUE(std::is_sorted(std::execution::par, std::begin(vi), std::end(vi)));
}

TEST(BoostRangesTest, boost_ranges) {
    const int mx_val = 32'000;
    auto gen = boost::irange(12, mx_val);
    std::random_device rd;
    std::mt19937 g(rd());
    std::vector<int> vi(boost::begin(gen), boost::end(gen));
    std::shuffle(std::begin(vi), std::end(vi), g);
    {
        boost::timer::auto_cpu_timer t;
        std::vector<int> rng = boost::copy_range<std::vector<int>>(vi | boost::adaptors::filtered([](int x) { return x % 2;}) );
        ASSERT_NE(rng.size(), vi.size());
    }
}

