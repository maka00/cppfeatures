#include <gtest/gtest.h>
#include <boost/range/adaptors.hpp>
#include <boost/range/irange.hpp>
#include <boost/timer/timer.hpp>
#include <random>
#include <algorithm>
#include <iterator>
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

TEST(PartitionTest, rotate) {
    const int mx_val = 1000;
    {
        auto gen = boost::irange(1, mx_val);
        boost::timer::auto_cpu_timer t;
        std::vector<int> vi(std::begin(gen), std::end(gen));
        EXPECT_EQ(vi.size(), mx_val - 1);
        auto first = std::begin(vi);
        auto last = std::end(vi);
        auto middle = first + std::distance(first, last) / 2;
        int item = *middle;
        std::rotate(first, middle, last);
        EXPECT_EQ(item, vi[0]);
    }
}

TEST(PartitionTest, rotate_copy) {
    const int mx_val = 1000;
    {
        auto gen = boost::irange(1, mx_val);
        boost::timer::auto_cpu_timer t;
        std::vector<int> vi(std::begin(gen), std::end(gen));
        EXPECT_EQ(vi.size(), mx_val - 1);
        auto first = std::begin(vi);
        auto last = std::end(vi);
        auto middle = first + std::distance(first, last) / 2;
        std::vector<int> vi_copy;
        std::rotate_copy(first,middle,last, std::back_inserter(vi_copy));
        EXPECT_EQ(vi_copy[0], *middle);
        EXPECT_EQ(vi_copy.size(), vi.size());
    }
}