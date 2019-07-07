#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "ptk/util/Container.hpp"

TEST_CASE("Constructors")
{
    // Testing Constructors with correct data.
    
    SECTION("Container()")
    {
        pTK::Container<int> cont;
        REQUIRE(cont.size() == 0);
    }
    
    SECTION("Container(T item)")
    {
        pTK::Container<int> cont(1);
        REQUIRE(cont.at(0) == 1);
        REQUIRE(cont.size() == 1);
    }
    
    SECTION("Initializer list")
    {
        pTK::Container<int> cont{1, 2, 3, 4, 5};
        REQUIRE(cont.size() == 5);
        REQUIRE(cont.at(0) == 1);
        REQUIRE(cont.at(1) == 2);
        REQUIRE(cont.at(2) == 3);
        REQUIRE(cont.at(3) == 4);
        REQUIRE(cont.at(4) == 5);
    }
}

TEST_CASE("Insert and Remove")
{
    // Testing insert and Remove.
    pTK::Container<int> cont;
    int integer = 5;
    
    SECTION("insert(const std::shared_ptr<Widget>& widget)")
    {
        cont.insert(integer);
        REQUIRE(cont.size() == 1);
    }
    
    SECTION("erase(const std::shared_ptr<Widget>& widget)")
    {
        cont.erase(integer);
        REQUIRE(cont.size() == 0);
    }
    
    SECTION("Duplicates on insert")
    {
        cont.insert(integer);
        cont.insert(integer);
        
        // Container allows duplicates.
        REQUIRE(cont.size() == 2);
    }
}

TEST_CASE("Access")
{
    // Testing access functions.
    pTK::Container<int> cont{1, 2, 3, 4, 5};
    pTK::Container<int> emptyContainer{};
    
    SECTION("Front")
    {
        REQUIRE(cont.front() == 1);
    }
    
    SECTION("Front")
    {
        REQUIRE(cont.back() == 5);
    }
    
    SECTION("Size")
    {
        REQUIRE(cont.size() == 5);
        REQUIRE(emptyContainer.size() == 0);
    }
    
    SECTION("Empty")
    {
        REQUIRE(cont.empty() == false);
        REQUIRE(emptyContainer.empty() == true);
    }
}

TEST_CASE("find (iterators)")
{
    // Testing find functions.
    pTK::Container<int> cont{1, 2, 3, 4, 5};;
    using iterator = pTK::Container<int>::iterator;
    using reverse_iterator = pTK::Container<int>::reverse_iterator;
    
    SECTION("find")
    {
        iterator it = cont.find(2);
        REQUIRE_FALSE(it == cont.end());
        REQUIRE((*it) == 2);
    }
    
    SECTION("rfind")
    {
        reverse_iterator it = cont.rfind(3);
        REQUIRE_FALSE(it == cont.rend());
        REQUIRE((*it) == 3);
    }
    
    SECTION("findIf")
    {
        iterator it = cont.findIf([](const int& item){
            return item == 5;
        });
        REQUIRE_FALSE(it == cont.end());
        REQUIRE((*it) == 5);
    }
    
    SECTION("rfindIf")
    {
        reverse_iterator it = cont.rfindIf([](const int& item){
            return item == 1;
        });
        REQUIRE_FALSE(it == cont.rend());
        REQUIRE((*it) == 1);
    }
}

TEST_CASE("forEach")
{
    // Testing for_each functions.
    pTK::Container<int> cont;
    int integers[3];
    integers[0] = 1;
    integers[1] = 2;
    integers[2] = 3;
    
    cont.insert(integers[0]);
    cont.insert(integers[1]);
    cont.insert(integers[2]);
    
    int i = 0;
    cont.forEach([&](const int& integer){
        REQUIRE(integer == integers[i]);
        i++;
    });
}

TEST_CASE("Copy and Assignment")
{
    // Testing Container Copy and Assignment.
    pTK::Container<int> cont;
    int integers[3];
    integers[0] = 1;
    integers[1] = 2;
    integers[2] = 3;
    
    cont.insert(integers[0]);
    cont.insert(integers[1]);
    cont.insert(integers[2]);
    
    SECTION("Copy")
    {
        pTK::Container<int> tmp = cont;
        
        unsigned int i = 0;
        bool equal = true;
        tmp.forEach([&](const int& integer){
            if (integers[i] != integer)
                equal = false;
            i++;
        });
        
        REQUIRE(tmp.size() == cont.size());
        REQUIRE(i == cont.size());
        REQUIRE(equal == true);
    }
    
    SECTION("Assignment")
    {
        pTK::Container<int> tmp;
        tmp = cont;
        
        unsigned int i = 0;
        bool equal = true;
        tmp.forEach([&](const int& integer){
            if (integers[i] != integer)
                equal = false;
            i++;
        });
        
        REQUIRE(tmp.size() == cont.size());
        REQUIRE(i == cont.size());
        REQUIRE(equal == true);
    }
}

TEST_CASE ("Comparison")
{
    // Testing Transformable Comparison.
    pTK::Container<int> cont;
    int integers[3];
    integers[0] = 1;
    integers[1] = 2;
    integers[2] = 3;
    
    cont.insert(integers[0]);
    cont.insert(integers[1]);
    cont.insert(integers[2]);
    
    pTK::Container<int> tmp = cont;
    pTK::Container<int> tmp2;
    pTK::Container<int> tmp3;
    tmp3.insert(integers[0]);
    pTK::Container<int> tmp4;
    tmp4.insert(integers[0]);
    
    SECTION("Equal")
    {
        REQUIRE(cont == tmp);
        REQUIRE(tmp3 == tmp4);
        REQUIRE_FALSE(cont == tmp2);
        REQUIRE_FALSE(cont == tmp3);
    }
    
    SECTION("Not Equal")
    {
        REQUIRE(cont != tmp2);
        REQUIRE(cont != tmp3);
        REQUIRE_FALSE(cont != tmp);
        REQUIRE_FALSE(tmp3 != tmp4);
    }
}

