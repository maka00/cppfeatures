//
// Created by mk on 27.09.21.
//

#include <gtest/gtest.h>
#include <boost/timer/timer.hpp>
#include <coroutine>
#include <algorithm>
#include <numeric>
#include <exception>
#include <thread>
#include <cassert>
#include <ranges>
using namespace std::literals;


class coro_task {
public:
    template<typename CoroType>
    struct coro_promise {
        auto get_return_object() {
            std::cout << __func__ << std::endl;
            return std::coroutine_handle<coro_promise<CoroType>>::from_promise(*this);
        }

        auto initial_suspend() {
            std::cout << __func__ << std::endl;
            return std::suspend_always{};
        }

        auto final_suspend() noexcept {
            std::cout << __func__ << std::endl;
            return std::suspend_always{};
        };

        auto unhandled_exception() {
            std::terminate();
        }

        void return_void() {
            std::cout << __func__ << std::endl;
        }
    };

    using promise_type = coro_promise<coro_task>;

private:
    std::coroutine_handle<promise_type> coro_handle;
public:
    coro_task(const auto &coroHandle) : coro_handle(coroHandle) {
        std::cout << __func__ << std::endl;
    }

    ~coro_task() {
        std::cout << __func__ << std::endl;
        if (coro_handle) {
            coro_handle.destroy();
        }
    }

    coro_task(const coro_task &) = delete;

    coro_task &operator=(const coro_task &) = delete;

    bool resume() const {
        std::cout << __func__ << std::endl;
        if (!coro_handle)
            return false;
        coro_handle.resume();
        return !coro_handle.done();
    }
};


coro_task say_hello() {
    std::cout << "Hello\n";
    co_await std::suspend_always();
    std::cout << "World\n";
    co_await std::suspend_always();
    std::cout << "!\n";
}

TEST(CoroTest, one) {
    {
        boost::timer::auto_cpu_timer t;
        testing::internal::CaptureStdout();
        coro_task hello = say_hello();
        while (hello.resume()) {
            std::this_thread::sleep_for(500ms);
        }
        std::string output = testing::internal::GetCapturedStdout();
        ASSERT_EQ(
                "get_return_object\ninitial_suspend\ncoro_task\nresume\nHello\nresume\nWorld\nresume\n!\nreturn_void\nfinal_suspend\n",
                output);
    }
}

// ----------------[ generator ]------------------------------
class coro_generator {
public:
    template<typename CoroType>
    struct coro_promise {
        int current_value;

        auto yield_value(int value) {
            current_value = value;
            return std::suspend_always{};
        }

        auto get_return_object() { return std::coroutine_handle<coro_promise<CoroType>>::from_promise(*this); }

        auto initial_suspend() { return std::suspend_always{}; }

        auto final_suspend() noexcept { return std::suspend_always{}; };

        auto unhandled_exception() { std::terminate(); }

        void return_void() {
            std::cout << __func__ << std::endl;
        }
    };

    using promise_type = coro_promise<coro_generator>;

private:
    std::coroutine_handle<promise_type> coro_handle;
public:
    coro_generator(const auto &coroHandle) : coro_handle(coroHandle) {
        std::cout << __func__ << std::endl;
    }

    ~coro_generator() {
        std::cout << __func__ << std::endl;
        if (coro_handle) {
            coro_handle.destroy();
        }
    }

    coro_generator(const coro_generator &) = delete;

    coro_generator(coro_generator &&rhs) : coro_handle(rhs.coro_handle) {
        rhs.coro_handle = nullptr;
    };

    coro_generator &operator=(const coro_generator &) = delete;

    struct iterator {
        std::coroutine_handle<promise_type> hndl; // nullptr at end
        iterator(auto p) : hndl{p} {}

        void getNext() {
            if (hndl) {
                hndl.resume();
                if (hndl.done()) {
                    hndl = nullptr;
                }
            }
        }

        int operator*() const {
            assert(hndl != nullptr);
            return hndl.promise().current_value;
        }

        iterator operator++() {
            getNext();
            return *this;
        }

        bool operator==(const iterator &i) const = default;
    };

    iterator begin() const {
        if (!coro_handle || coro_handle.done()) {
            return iterator{nullptr};
        }
        iterator it{coro_handle};
        it.getNext();
        return it;
    }

    iterator end() const {
        return iterator{nullptr};
    }

};

coro_generator generate_one(int max) {

    for (int i = 0; i <= max; i++) {
        co_yield i;
    }
}

TEST(CoroTest, generator_one) {
    static constexpr int max = 12;
    auto gen = generate_one(max);
    auto result = std::accumulate(std::begin(gen), std::end(gen), 0, std::plus());
    std::vector<int> comp(max + 1);
    std::iota(std::begin(comp), std::end(comp),0);
    ASSERT_EQ(std::accumulate(std::begin(comp), std::end(comp), 0, std::plus()), result);
}