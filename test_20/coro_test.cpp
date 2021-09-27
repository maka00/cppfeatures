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
        if(coro_handle) {
            coro_handle.destroy();
        }
    }
    coro_task(const coro_task&) = delete;
    coro_task& operator=(const coro_task&) = delete;
    bool resume() const {
        std::cout << __func__ << std::endl;
        if(!coro_handle)
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
using namespace std::literals;

TEST(CoroTest, one) {
    {
        boost::timer::auto_cpu_timer t;
        testing::internal::CaptureStdout();
        coro_task hello = say_hello();
        while (hello.resume()) {
            std::this_thread::sleep_for(500ms);
        }
        std::string output = testing::internal::GetCapturedStdout();
        ASSERT_EQ("get_return_object\ninitial_suspend\ncoro_task\nresume\nHello\nresume\nWorld\nresume\n!\nreturn_void\nfinal_suspend\n", output);
    }
}