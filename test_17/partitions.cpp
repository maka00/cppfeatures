#include <gtest/gtest.h>
#include <boost/range/adaptors.hpp>
#include <boost/range/irange.hpp>
#include <boost/timer/timer.hpp>
#include <random>
#include <algorithm>
#include <execution>
#include <iostream>

TEST(PartitionsTest, partition_one) {
    const int mx_val = 1000;
    // boost auto_cpu_timer outputs the time spend in this scope
    {
        auto gen = boost::irange(1, mx_val);
        boost::timer::auto_cpu_timer t;
        std::vector<int> vi(std::begin(gen), std::end(gen));
        EXPECT_EQ(vi.size(), mx_val - 1);
        auto it = std::partition(std::begin(vi), std::end(vi), [](int a) { return a % 2; });
        EXPECT_FALSE(std::is_sorted(it, std::end(vi)));
        EXPECT_FALSE(std::is_sorted(std::begin(vi), it));
    }
}

TEST(PartitionsTest, stable_partition_one) {
    const int mx_val = 1000;
    {
        auto gen = boost::irange(1, mx_val);
        boost::timer::auto_cpu_timer t;
        std::vector<int> vi(std::begin(gen), std::end(gen));
        EXPECT_EQ(vi.size(), mx_val - 1);
        auto it = std::stable_partition(std::begin(vi), std::end(vi), [](int a) { return a % 2; });
        EXPECT_TRUE(std::is_sorted(it, std::end(vi)));
        EXPECT_TRUE(std::is_sorted(std::begin(vi), it));
    }
}
