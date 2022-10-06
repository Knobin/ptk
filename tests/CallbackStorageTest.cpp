//
//  tests/CallbackStorageTest.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2022-10-04.
//

// Catch2 Headers
#include "catch2/catch_test_macros.hpp"

// pTK Headers
#include "ptk/core/CallbackStorage.hpp"

// C++ Headers
#include <cstdint>

static auto ZeroCallback = [](){ return false; };

TEST_CASE ("CallbackContainer")
{
    SECTION("Add")
    {
        pTK::CallbackContainer<bool()> container{};
        container.addCallback(0, ZeroCallback);
        container.addCallback(1, ZeroCallback);
        container.addCallback(2, ZeroCallback);
        REQUIRE(container.size() == 3);
    }

    SECTION("Trigger")
    {
        pTK::CallbackContainer<bool(std::size_t&)> container{};
        container.addCallback(0, [](std::size_t& var){ var += 1; return false; });
        container.addCallback(1, [](std::size_t& var){ var += 2; return false; });
        container.addCallback(2, [](std::size_t& var){ var += 4; return false; });

        std::size_t count = 0;
        container.triggerCallbacks(count);
        REQUIRE(count == 7);
    }

    SECTION("Remove")
    {
        pTK::CallbackContainer<bool(std::size_t&)> container{};
        container.addCallback(0, [](std::size_t& var){ var += 1; return false; });
        container.addCallback(1, [](std::size_t& var){ var += 2; return false; });
        container.addCallback(2, [](std::size_t& var){ var += 4; return false; });

        std::size_t count = 0;
        container.triggerCallbacks(count);
        REQUIRE(count == 7);

        count = 0;
        container.removeCallback(0);
        container.triggerCallbacks(count);
        REQUIRE(container.size() == 2);
        REQUIRE(count == 6);

        count = 0;
        container.removeCallback(1);
        container.triggerCallbacks(count);
        REQUIRE(container.size() == 1);
        REQUIRE(count == 4);

        count = 0;
        container.removeCallback(2);
        container.triggerCallbacks(count);
        REQUIRE(container.size() == 0);
        REQUIRE(count == 0);
    }

    SECTION("Copy")
    {
        pTK::CallbackContainer<bool(std::size_t&)> container{};
        container.addCallback(0, [](std::size_t& var){ var += 1; return false; });
        container.addCallback(1, [](std::size_t& var){ var += 2; return false; });
        container.addCallback(2, [](std::size_t& var){ var += 4; return false; });

        pTK::CallbackContainer<bool(std::size_t&)> copy = container.clone();

        // Original.
        std::size_t count = 0;
        container.triggerCallbacks(count);
        REQUIRE(container.size() == 3);
        REQUIRE(count == 7);

        // Copy.
        count = 0;
        container.triggerCallbacks(count);
        REQUIRE(container.size() == 3);
        REQUIRE(count == 7);
    }

    SECTION("Clear")
    {
        pTK::CallbackContainer<bool()> container{};
        container.addCallback(0, ZeroCallback);
        container.addCallback(1, ZeroCallback);
        container.addCallback(2, ZeroCallback);

        REQUIRE(container.size() == 3);
        container.clear();
        REQUIRE(container.size() == 0);
    }
}

TEST_CASE("CallbackIndexGen")
{
    REQUIRE(pTK::CallbackIndexGen::GetIndex<int8_t, bool()>() == 0);
    REQUIRE(pTK::CallbackIndexGen::GetIndex<int8_t, bool()>() == 0); // Same as the above.

    REQUIRE(pTK::CallbackIndexGen::GetIndex<int8_t, bool(int)>() == 1);
    REQUIRE(pTK::CallbackIndexGen::GetIndex<int8_t, bool(unsigned int)>() == 2);

    REQUIRE(pTK::CallbackIndexGen::GetIndex<int8_t, bool()>() == 0); // Same as the first.
    REQUIRE(pTK::CallbackIndexGen::GetIndex<uint8_t, bool()>() == 3);
}

TEST_CASE("CallbackStorage")
{
    SECTION("Add")
    {
        pTK::CallbackStorage storage{};

        REQUIRE(1 == storage.addCallback<int8_t, bool()>(ZeroCallback));
        REQUIRE(2 == storage.addCallback<int8_t, bool()>(ZeroCallback));
        REQUIRE(3 == storage.addCallback<int8_t, bool()>(ZeroCallback));
        REQUIRE(storage.size() == 1);
        REQUIRE(storage.count() == 3);

        REQUIRE(4 == storage.addCallback<uint8_t, bool()>(ZeroCallback));
        REQUIRE(5 == storage.addCallback<uint8_t, bool()>(ZeroCallback));
        REQUIRE(storage.size() == 2);
        REQUIRE(storage.count() == 5);
    }

    SECTION("Trigger")
    {
        pTK::CallbackStorage storage{};
        REQUIRE(6 == storage.addCallback<int8_t, bool(std::size_t&)>([](std::size_t& var){ var += 1; return false; }));
        REQUIRE(7 == storage.addCallback<int8_t, bool(std::size_t&)>([](std::size_t& var){ var += 2; return false; }));
        REQUIRE(8 == storage.addCallback<uint8_t, bool(std::size_t&)>([](std::size_t& var){ var += 4; return false; }));
        REQUIRE(storage.size() == 2);
        REQUIRE(storage.count() == 3);

        std::size_t count = 0;
        storage.triggerCallbacks<int8_t, bool(std::size_t&)>(count);
        REQUIRE(count == 3);

        count = 0;
        storage.triggerCallbacks<uint8_t, bool(std::size_t&)>(count);
        REQUIRE(count == 4);
    }

    SECTION("Remove")
    {
        pTK::CallbackStorage storage{};
        REQUIRE(9 == storage.addCallback<int8_t, bool(std::size_t&)>([](std::size_t& var){ var += 1; return false; }));
        REQUIRE(10 == storage.addCallback<int8_t, bool(std::size_t&)>([](std::size_t& var){ var += 2; return false; }));
        REQUIRE(11 == storage.addCallback<uint8_t, bool(std::size_t&)>([](std::size_t& var){ var += 4; return false; }));
        REQUIRE(storage.size() == 2);
        REQUIRE(storage.count() == 3);

        // None removed, 9,10,11 exists.
        std::size_t count = 0;
        storage.triggerCallbacks<int8_t, bool(std::size_t&)>(count);
        REQUIRE(count == 3);
        storage.triggerCallbacks<uint8_t, bool(std::size_t&)>(count);
        REQUIRE(count == 7);

        // Removing 9. 10,11 exists.
        count = 0;
        storage.removeCallback<int8_t, bool(std::size_t&)>(9);
        storage.triggerCallbacks<int8_t, bool(std::size_t&)>(count);
        REQUIRE(storage.size() == 2);
        REQUIRE(count == 2);
        REQUIRE(storage.count() == 2);

        // Removing 11. 10 exists.
        count = 0;
        storage.removeCallback<uint8_t, bool(std::size_t&)>(11);
        storage.triggerCallbacks<uint8_t, bool(std::size_t&)>(count);
        REQUIRE(storage.size() == 1);
        REQUIRE(count == 0);
        REQUIRE(storage.count() == 1);

        // Removing 10. None left.
        count = 0;
        storage.removeCallback<int8_t, bool(std::size_t&)>(10);
        storage.triggerCallbacks<int8_t, bool(std::size_t&)>(count);
        REQUIRE(storage.size() == 0);
        REQUIRE(count == 0);
        REQUIRE(storage.count() == 0);

        // Removing out of range index.
        count = 0;
        storage.removeCallback<int8_t, bool(std::size_t&)>(100);
        storage.triggerCallbacks<int8_t, bool(std::size_t&)>(count);
        REQUIRE(storage.size() == 0);
        REQUIRE(count == 0);
        REQUIRE(storage.count() == 0);
    }

    SECTION("Copy")
    {
        pTK::CallbackStorage storage{};
        REQUIRE(12 == storage.addCallback<int8_t, bool(std::size_t&)>([](std::size_t& var){ var += 1; return false; }));
        REQUIRE(13 == storage.addCallback<int8_t, bool(std::size_t&)>([](std::size_t& var){ var += 2; return false; }));
        REQUIRE(14 == storage.addCallback<uint8_t, bool(std::size_t&)>([](std::size_t& var){ var += 4; return false; }));
        REQUIRE(storage.count() == 3);

        pTK::CallbackStorage copy = storage.clone();
        REQUIRE(storage.count() == 3);
        REQUIRE(copy.count() == 3);

        // Original.
        std::size_t count = 0;
        storage.triggerCallbacks<int8_t, bool(std::size_t&)>(count);
        REQUIRE(count == 3);
        storage.triggerCallbacks<uint8_t, bool(std::size_t&)>(count);
        REQUIRE(count == 7);

        // Copy.
        count = 0;
        copy.triggerCallbacks<int8_t, bool(std::size_t&)>(count);
        REQUIRE(count == 3);
        copy.triggerCallbacks<uint8_t, bool(std::size_t&)>(count);
        REQUIRE(count == 7);

        // Change check.
        REQUIRE(15 == copy.addCallback<uint8_t, bool(std::size_t&)>([](std::size_t& var){ var += 8; return false; }));
        REQUIRE(copy.count() == 4);
        REQUIRE(storage.count() == 3);
        count = 0;
        copy.triggerCallbacks<int8_t, bool(std::size_t&)>(count);
        copy.triggerCallbacks<uint8_t, bool(std::size_t&)>(count);
        REQUIRE(count == 15);
        count = 0;
        storage.triggerCallbacks<int8_t, bool(std::size_t&)>(count);
        storage.triggerCallbacks<uint8_t, bool(std::size_t&)>(count);
        REQUIRE(count == 7);
    }

    SECTION("Clear")
    {
        pTK::CallbackStorage storage{};
        REQUIRE(16 == storage.addCallback<int8_t, bool()>(ZeroCallback));
        REQUIRE(17 == storage.addCallback<int8_t, bool()>(ZeroCallback));
        REQUIRE(18 == storage.addCallback<uint8_t, bool()>(ZeroCallback));
        REQUIRE(19 == storage.addCallback<uint8_t, bool()>(ZeroCallback));

        REQUIRE(storage.size() == 2);
        REQUIRE(storage.count() == 4);
        storage.clear();
        REQUIRE(storage.size() == 0);
        REQUIRE(storage.count() == 0);
    }

    SECTION("Remove bucket")
    {
        pTK::CallbackStorage storage{};
        REQUIRE(20 == storage.addCallback<int8_t, bool()>(ZeroCallback));
        REQUIRE(21 == storage.addCallback<int8_t, bool()>(ZeroCallback));
        REQUIRE(22 == storage.addCallback<uint8_t, bool()>(ZeroCallback));
        REQUIRE(23 == storage.addCallback<uint8_t, bool()>(ZeroCallback));

        REQUIRE(storage.size() == 2);
        REQUIRE(storage.count() == 4);
        storage.removeCallbacks<int8_t, bool()>();
        REQUIRE(storage.size() == 1);
        REQUIRE(storage.count() == 2);
        storage.removeCallbacks<uint8_t, bool()>();
        REQUIRE(storage.size() == 0);
        REQUIRE(storage.count() == 0);
    }

    SECTION("Auto-Removing Callback")
    {
        pTK::CallbackStorage storage{};
        REQUIRE(24 == storage.addCallback<int8_t, bool(std::size_t&)>([](std::size_t& var){ var += 1; return false; }));
        REQUIRE(25 == storage.addCallback<int8_t, bool(std::size_t&)>([](std::size_t& var){ var += 2; return true; }));
        REQUIRE(26 == storage.addCallback<uint8_t, bool(std::size_t&)>([](std::size_t& var){ var += 4; return true; }));
        REQUIRE(storage.size() == 2);
        REQUIRE(storage.count() == 3);

        std::size_t count = 0;
        storage.triggerCallbacks<int8_t, bool(std::size_t&)>(count);
        REQUIRE(count == 3);
        REQUIRE(storage.size() == 2);
        REQUIRE(storage.count() == 2);

        count = 0;
        storage.triggerCallbacks<uint8_t, bool(std::size_t&)>(count);
        REQUIRE(count == 4);
        REQUIRE(storage.size() == 1);
        REQUIRE(storage.count() == 1);
    }

    SECTION("Get Callbacks")
    {
        pTK::CallbackStorage storage{};
        REQUIRE(27 == storage.addCallback<int8_t, bool(std::size_t&)>([](std::size_t& var){ var += 1; return false; }));
        REQUIRE(28 == storage.addCallback<int8_t, bool(std::size_t&)>([](std::size_t& var){ var += 2; return true; }));
        REQUIRE(29 == storage.addCallback<uint8_t, bool(std::size_t&)>([](std::size_t& var){ var += 4; return true; }));

        pTK::CallbackContainer<bool(std::size_t&)>* callbacks1 = storage.getCallbacks<int8_t, bool(std::size_t&)>();
        REQUIRE(callbacks1->size() == 2);
        std::size_t count = 0;
        callbacks1->triggerCallbacks(count);
        REQUIRE(count == 3);

        const pTK::CallbackStorage *storagePtr = &storage;
        const pTK::CallbackContainer<bool(std::size_t&)>* callbacks2 = storagePtr->getCallbacks<int8_t, bool(std::size_t&)>();
        REQUIRE(callbacks2->size() == 2);
        count = 0;
        callbacks2->triggerCallbacks(count);
    }
}