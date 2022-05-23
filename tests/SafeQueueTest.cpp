// Catch2 Headers
#include "catch2/catch_test_macros.hpp"

// pTK Headers
#include "ptk/util/SafeQueue.hpp"

TEST_CASE("Constructors")
{
    pTK::SafeQueue<int> val{};
    val.push(1);
    val.push(2);
    val.push(3);

    SECTION("Default")
    {
        pTK::SafeQueue<int> queue{};
        REQUIRE(queue.size() == 0);
        REQUIRE(queue.empty() == true);
    }

    SECTION("Copy")
    {
        pTK::SafeQueue<int> queue{val};

        REQUIRE(queue.size() == 3);
        REQUIRE(queue.empty() == false);
        REQUIRE(queue == val);
    }

    SECTION("Move")
    {
        pTK::SafeQueue<int> q1{val};
        pTK::SafeQueue<int> q2{std::move(q1)};

        REQUIRE(q1.size() == 0);
        REQUIRE(q1.empty() == true);
        REQUIRE_FALSE(q1 == val);

        REQUIRE(q2.size() == 3);
        REQUIRE(q2.empty() == false);
        REQUIRE(q2 == val);
    }
}

TEST_CASE("Assignment operators")
{
    pTK::SafeQueue<int> queue{};
    queue.push(1);
    queue.push(2);
    queue.push(3);

    SECTION("Copy")
    {
        pTK::SafeQueue<int> q1{};
        q1 = queue;

        REQUIRE(q1.size() == 3);
        REQUIRE(q1.empty() == false);
        REQUIRE(q1 == queue);
    }

    SECTION("Move")
    {
        pTK::SafeQueue<int> q1{queue};
        pTK::SafeQueue<int> q2{};
        q2 = std::move(q1);

        REQUIRE(q1.size() == 0);
        REQUIRE(q1.empty() == true);
        REQUIRE_FALSE(q1 == queue);

        REQUIRE(q2.size() == 3);
        REQUIRE(q2.empty() == false);
        REQUIRE(q2 == queue);
    }
}

TEST_CASE("Comparison operators")
{
    pTK::SafeQueue<int> queue{};
    queue.push(1);
    queue.push(2);
    queue.push(3);

    pTK::SafeQueue<int> q1{queue};
    pTK::SafeQueue<int> q2{};
    pTK::SafeQueue<int> q3{queue};

    SECTION("Equal")
    {
        REQUIRE(q1 == q3);
        REQUIRE(q3 == q1);
        REQUIRE_FALSE(q1 == q2);
        REQUIRE_FALSE(q2 == q1);
    }

    SECTION("Not equal")
    {
        REQUIRE_FALSE(q1 != q3);
        REQUIRE_FALSE(q3 != q1);
        REQUIRE(q1 != q2);
        REQUIRE(q2 != q1);
    }

    SECTION("Less than")
    {
        REQUIRE(q2 < q1);
        REQUIRE_FALSE(q1 < q2);
        REQUIRE_FALSE(q1 < q3);
    }

    SECTION("Greater than")
    {
        REQUIRE(q1 > q2);
        REQUIRE_FALSE(q2 > q1);
        REQUIRE_FALSE(q1 > q3);
    }

    SECTION("Less than or equal to")
    {
        REQUIRE(q2 <= q1);
        REQUIRE(q1 <= q3);
        REQUIRE_FALSE(q1 <= q2);
    }

    SECTION("Greater than or equal to")
    {
        REQUIRE(q1 >= q2);
        REQUIRE(q1 >= q3);
        REQUIRE_FALSE(q2 >= q1);
    }
}

TEST_CASE("Modifiers")
{
    pTK::SafeQueue<int> queue{};

    SECTION("push")
    {
        int a = 10;
        queue.push(a);
        REQUIRE(queue.front() == 10);
        REQUIRE(queue.size() == 1);
        REQUIRE(queue.empty() == false);

        queue.push(5);
        REQUIRE(queue.back() == 5);
        REQUIRE(queue.size() == 2);
        REQUIRE(queue.empty() == false);
    }

    SECTION("emplace")
    {
        queue.emplace(5);

        REQUIRE(queue.front() == 5);
        REQUIRE(queue.size() == 1);
        REQUIRE(queue.empty() == false);
    }

    SECTION("pop")
    {
        queue.push(1);
        queue.push(2);
        REQUIRE(queue.size() == 2);

        queue.pop();
        REQUIRE(queue.size() == 1);

        queue.pop();
        REQUIRE(queue.size() == 0);
    }

    SECTION("const_reference front")
    {
        queue.push(5);
        REQUIRE(queue.front() == 5);

        queue.push(10);
        REQUIRE(queue.front() == 5);

        queue.pop();
        REQUIRE(queue.front() == 10);
    }

    SECTION("const_reference back")
    {
        queue.push(5);
        REQUIRE(queue.back() == 5);

        queue.push(10);
        REQUIRE(queue.back() == 10);

        queue.pop();
        REQUIRE(queue.back() == 10);
    }

    SECTION("reference front")
    {
        queue.push(5);
        queue.push(15);
        REQUIRE(queue.front() == 5);
        REQUIRE(queue.back() == 15);
        queue.front() = 10;
        REQUIRE(queue.front() == 10);
        REQUIRE(queue.back() == 15);
    }

    SECTION("reference back")
    {
        queue.push(5);
        queue.push(15);
        REQUIRE(queue.front() == 5);
        REQUIRE(queue.back() == 15);
        queue.back() = 10;
        REQUIRE(queue.front() == 5);
        REQUIRE(queue.back() == 10);
    }
}

TEST_CASE("Capacity")
{
    pTK::SafeQueue<int> queue{};
    
    SECTION("empty")
    {
        REQUIRE(queue.empty() == true);

        queue.push(1);
        REQUIRE(queue.empty() == false);

        queue.pop();
        REQUIRE(queue.empty() == true);
    }

    SECTION("size")
    {
        REQUIRE(queue.size() == 0);

        queue.push(1);
        REQUIRE(queue.size() == 1);
        queue.push(2);
        REQUIRE(queue.size() == 2);

        queue.pop();
        queue.pop();
        REQUIRE(queue.size() == 0);
    }
}
