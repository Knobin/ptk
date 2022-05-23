// Catch2 Headers
#include "catch2/catch_test_macros.hpp"

// pTK Headers
#include "ptk/util/Semaphore.hpp"

// C++ Headers
#include <thread>
#include <chrono>

TEST_CASE("Constructors")
{
    pTK::Semaphore sema{5};
    
    REQUIRE(sema.getvalue() == 5);
}

TEST_CASE("Decrement and Increment")
{
    SECTION("Decrement")
    {
        pTK::Semaphore sema{2};
        REQUIRE(sema.getvalue() == 2);
        sema.wait();
        REQUIRE(sema.getvalue() == 1);
    }
    
    SECTION("Increment")
    {
        pTK::Semaphore sema{0};
        REQUIRE(sema.getvalue() == 0);
        sema.post();
        REQUIRE(sema.getvalue() == 1);
    }
}

TEST_CASE("Wait")
{
    SECTION("wait()")
    {
        pTK::Semaphore sema{0};
        auto start = std::chrono::high_resolution_clock::now();
        std::thread t1{[&sema]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            sema.post();
        }};
        sema.wait();
        t1.join();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        REQUIRE((int)duration.count() >= 100);
    }
    
    SECTION("trywait(), sema = 0")
    {
        pTK::Semaphore sema{0};
        REQUIRE(sema.trywait() == -1);
        REQUIRE(sema.getvalue() == 0);
    }
    
    SECTION("trywait(), sema > 0")
    {
        pTK::Semaphore sema{1};
        REQUIRE(sema.trywait() == 0);
        REQUIRE(sema.getvalue() == 0);
    }
}

template<typename T>
int sema_timedwait_time(pTK::Semaphore& sema, int time)
{
    auto start = std::chrono::high_resolution_clock::now();
    int return_val = sema.timedwait(T(time));
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<T>(end - start);
    REQUIRE((int)duration.count() >= time);
    
    return return_val;
}

TEST_CASE("Timedwait")
{
    SECTION("Check return value on full wait")
    {
        pTK::Semaphore sema{0};
        REQUIRE(sema.timedwait(std::chrono::milliseconds(50)) == -1);
    }
    
    SECTION("Check return value on full wait")
    {
        pTK::Semaphore sema{1};
        REQUIRE(sema.timedwait(std::chrono::milliseconds(50)) == 0);
    }
    
    SECTION("Check if timedwait waits the correct times")
    {
        pTK::Semaphore sema{0};
        REQUIRE(sema_timedwait_time<std::chrono::nanoseconds>(sema,  1000000000) == -1);
        REQUIRE(sema_timedwait_time<std::chrono::microseconds>(sema, 1000000) == -1);
        REQUIRE(sema_timedwait_time<std::chrono::milliseconds>(sema, 1000) == -1);
        REQUIRE(sema_timedwait_time<std::chrono::seconds>(sema, 1) == -1);
        REQUIRE(sema.getvalue() == 0);
    }
    
    SECTION("post, when waiting")
    {
        pTK::Semaphore sema{0};
        auto start = std::chrono::high_resolution_clock::now();
        std::thread t1{[&sema]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            sema.post();
        }};
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        REQUIRE(sema.timedwait(std::chrono::milliseconds(100)) == 0);
        REQUIRE((int)duration.count() < 100);
        REQUIRE(sema.getvalue() == 0);
        t1.join();
    }
}
