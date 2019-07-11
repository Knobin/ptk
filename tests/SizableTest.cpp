#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "ptk/core/Sizable.hpp"

TEST_CASE("Constructors")
{
    // Testing Constructors with correct data.
    
    SECTION("Sizable()")
    {
        pTK::Sizable size;
        REQUIRE(size.getMinSize() == pTK::Size(0.0f, 0.0f));
        REQUIRE(size.getSize() == pTK::Size(0.0f, 0.0f));
        REQUIRE(size.getMaxSize() == pTK::Size(0.0f, 0.0f));
    }
}

TEST_CASE("Getters and Setters")
{
    // Testing Getters and Setters.
    pTK::Sizable size;
    
    SECTION("minSize")
    {
        pTK::Size minSize(10.0f, 2.0f);
        size.setMinSize(minSize);
        
        REQUIRE(size.getMinSize() == minSize);
    }
    
    SECTION("size")
    {
        pTK::Size s(1200.0f, 142.0f);
        size.setSize(s);
        
        REQUIRE(size.getSize() == s);
    }
    
    SECTION("maxSize")
    {
        pTK::Size maxSize(2420.0f, 420.0f);
        size.setMaxSize(maxSize);
        
        REQUIRE(size.getMaxSize() == maxSize);
    }
}

TEST_CASE("Copy and Assignment")
{
    // Testing Container Copy and Assignment.
    pTK::Sizable size;
    pTK::Size s1(10.0f, 20.0f);
    pTK::Size s2(15.0f, 30.0f);
    pTK::Size s3(50.0f, 100.0f);
    size.setMinSize(s1);
    size.setSize(s2);
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
    pTK::Size s1(10.0f, 20.0f);
    pTK::Size s2(15.0f, 30.0f);
    pTK::Size s3(50.0f, 100.0f);
    size.setMinSize(s1);
    size.setSize(s2);
    size.setMaxSize(s3);
    
    pTK::Sizable tmp = size;
    pTK::Sizable tmp2;
    pTK::Sizable tmp3;
    tmp3.setSize(s2);
    pTK::Sizable tmp4;
    tmp4.setMinSize(s1);
    tmp4.setSize(s2);
    pTK::Sizable tmp5;
    tmp5.setMinSize(s1);
    tmp5.setSize(s2);
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


