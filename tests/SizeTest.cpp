#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "ptk/util/Size.hpp"

TEST_CASE("Constructors")
{
    // Testing Constructors with correct data.
    int cWidth = 50;
    int cHeight = 300;
    
    SECTION("Size()")
    {
        pTK::Size size;
        
        REQUIRE(size.width == 0);
        REQUIRE(size.height == 0);
    }
    
    SECTION("Size(int width, int height)")
    {
        pTK::Size size{cWidth, cHeight};
        
        REQUIRE(size.width == cWidth);
        REQUIRE(size.height == cHeight);
    }
}

TEST_CASE("Getters and Setters")
{
    // Testing Getters and Setters.
    int cWidth = 55;
    int cHeight = 350;
    
    SECTION("setSize(int width, int height)")
    {
        pTK::Size size;
        size.setSize(cWidth, cHeight);
        REQUIRE(size.width == cWidth);
        REQUIRE(size.height == cHeight);
    }
}

TEST_CASE("Copy and Assignment")
{
    // Testing Rectangle Copy and Assignment.
    int cWidth = 949;
    int cHeight = 84;
    
    pTK::Size size;
    size.setSize(cWidth, cHeight);
    
    SECTION("Copy")
    {
        pTK::Size tmp = size;
        REQUIRE(tmp.width == cWidth);
        REQUIRE(tmp.height == cHeight);
    }
    
    SECTION("Assignment")
    {
        pTK::Size tmp;
        tmp = size;
        REQUIRE(tmp.width == cWidth);
        REQUIRE(tmp.height == cHeight);
    }
}

TEST_CASE ("Comparison")
{
    // Testing Transformable Comparison.
    int cWidth = 57;
    int cHeight = 384;
    
    pTK::Size size;
    size.setSize(cWidth, cHeight);
    
    pTK::Size s1 = size;
    
    pTK::Size s2;
    s2.height = cHeight;
    
    pTK::Size s3;
    s3.width = cWidth;
    
    SECTION("Equal")
    {
        REQUIRE(size == s1);
        REQUIRE_FALSE(size == s2);
        REQUIRE_FALSE(size == s3);
    }
    
    SECTION("Not Equal")
    {
        REQUIRE(size != s2);
        REQUIRE(size != s3);
        REQUIRE(s2 != s3);
        REQUIRE_FALSE(size != s1);
    }
}


