#include <gtest/gtest.h>
#include <boost/timer/timer.hpp>
#include <random>
#include <algorithm>
#include <numeric>
#include <ranges>

TEST(RangesTest, one) {
    // boost auto_cpu_timer outputs the time spend in this scope
    {
        boost::timer::auto_cpu_timer t;
        std::ranges::for_each(std::ranges::iota_view{1, 10}, [](int i) { ASSERT_GT(i, 0); });
    }
}

