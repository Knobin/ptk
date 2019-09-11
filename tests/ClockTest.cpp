#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

// Local Headers
#include "ptk/util/Clock.hpp"

// C++ Headers
#include <thread>
#include <chrono>

TEST_CASE("Constructors")
{
    pTK::Clock clk;
    
    int64 ns = clk.nanoseconds();
    int64 ns2 = clk.nanoseconds();
    REQUIRE(ns2 > ns);
}

TEST_CASE("Return values")
{
    pTK::Clock clk;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    clk.stop();
    
    REQUIRE((clk.nanoseconds() / 1000000000) == 1);
    REQUIRE((clk.microseconds() / 1000000) == 1);
    REQUIRE((clk.milliseconds() / 1000) == 1);
    REQUIRE(clk.seconds() == 1);
    
    // Hours will be hard to test...
}

TEST_CASE("Stop")
{
    pTK::Clock clk;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    clk.stop();
    REQUIRE(clk.milliseconds() >= 50);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    REQUIRE(clk.milliseconds() < 100);
}
