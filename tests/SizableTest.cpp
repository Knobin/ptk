#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "ptk/core/Sizable.hpp"

/** Need to add more tests.
 
    Especially to the special cases section.
 */

#define ZERO pTK::Size(0, 0)

TEST_CASE("Constructors")
{
    // Testing Constructors with correct data.
    
    SECTION("Sizable()")
    {
        pTK::Sizable size;
        REQUIRE(size.getMinSize() == ZERO);
        REQUIRE(size.getSize() == ZERO);
        REQUIRE(size.getMaxSize() == ZERO);
    }
    
    SECTION("Sizable(const Size& size)")
    {
        pTK::Size s1(10, 20);
        pTK::Sizable size(s1);
        REQUIRE(size.getMinSize() == ZERO);
        REQUIRE(size.getSize() == pTK::Size(10, 20));
        REQUIRE(size.getMaxSize() == pTK::Size(10, 20)); // Max size not set.
    }
}

TEST_CASE("Getters and Setters")
{
    // Testing Getters and Setters.
    pTK::Sizable size;
    
    SECTION("minSize")
    {
        pTK::Size minSize(10, 2);
        size.setMinSize(minSize);
        
        REQUIRE_FALSE(size.getMinSize() == minSize);
        REQUIRE(size.getMinSize() == ZERO);
        
        pTK::Sizable shouldWork;
        shouldWork.setSize(pTK::Size(20, 10));
        shouldWork.setMinSize(minSize);
        REQUIRE(shouldWork.getMinSize() == minSize);
        REQUIRE(shouldWork.getSize() == pTK::Size(20, 10));
        REQUIRE(shouldWork.getMaxSize() == pTK::Size(20, 10));
    }
    
    SECTION("size")
    {
        pTK::Size s(1200, 142);
        size.setSize(s);
        
        REQUIRE(size.getMinSize() == ZERO);
        REQUIRE(size.getSize() == s);
        REQUIRE(size.getMaxSize() == s);
    }
    
    SECTION("maxSize")
    {
        pTK::Size maxSize(2420, 420);
        size.setMaxSize(maxSize);
        
        REQUIRE(size.getMinSize() == ZERO);
        REQUIRE(size.getSize() == ZERO);
        REQUIRE(size.getMaxSize() == maxSize);
    }
}

TEST_CASE("Special Size cases")
{
    // Testing Getters and Setters for size.
    
    SECTION("Standard Size")
    {
        pTK::Sizable size;
        
        // Minimal size bigger than (0, 0)
        pTK::Size minSize(20, 10);
        size.setMinSize(minSize);
        REQUIRE_FALSE(size.getMinSize() == minSize);
        REQUIRE(size.getMinSize() == pTK::Size(0, 0));
    }
}

TEST_CASE("Copy and Assignment")
{
    // Testing Container Copy and Assignment.
    pTK::Sizable size;
    pTK::Size s1(10, 20);
    pTK::Size s2(15, 30);
    pTK::Size s3(50, 100);
    size.setSize(s2);
    size.setMinSize(s1);
    size.setMaxSize(s3);
    
    SECTION("Copy")
    {
        pTK::Sizable tmp = size;
        
        REQUIRE(size.getMinSize() == s1);
        REQUIRE(size.getSize() == s2);
        REQUIRE(size.getMaxSize() == s3);
    }
    
    SECTION("Assignment")
    {
        pTK::Sizable tmp;
        tmp = size;
        
        REQUIRE(size.getMinSize() == s1);
        REQUIRE(size.getSize() == s2);
        REQUIRE(size.getMaxSize() == s3);
    }
}

TEST_CASE ("Comparison")
{
    // Testing Transformable Comparison.
    pTK::Sizable size;
    pTK::Size s1(10, 20);
    pTK::Size s2(15, 30);
    pTK::Size s3(50, 100);
    size.setSize(s2);
    size.setMinSize(s1);
    size.setMaxSize(s3);
    
    pTK::Sizable tmp = size;
    pTK::Sizable tmp2;
    pTK::Sizable tmp3;
    tmp3.setSize(s2);
    pTK::Sizable tmp4;
    tmp4.setSize(s2);
    tmp4.setMinSize(s1);
    pTK::Sizable tmp5;
    tmp5.setSize(s2);
    tmp5.setMinSize(s1);
    tmp5.setMaxSize(s3);

    SECTION("Equal")
    {
        REQUIRE(size == tmp);
        REQUIRE(size == tmp5);
        REQUIRE_FALSE(tmp3 == tmp4);
        REQUIRE_FALSE(size == tmp2);
        REQUIRE_FALSE(size == tmp3);
    }
    
    SECTION("Not Equal")
    {
        REQUIRE(size != tmp2);
        REQUIRE(size != tmp3);
        REQUIRE(tmp3 != tmp4);
        REQUIRE_FALSE(size != tmp);
        REQUIRE_FALSE(size != tmp5);
    }
}


