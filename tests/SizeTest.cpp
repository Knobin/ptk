#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "ptk/util/Size.hpp"

TEST_CASE("Constructors")
{
    // Testing Constructors with correct data.
    float cWidth = 50.0f;
    float cHeight = 300.0f;
    
    SECTION("Size()")
    {
        pTK::Size size;
        
        REQUIRE(size.getWidth() == 0.0f);
        REQUIRE(size.getHeight() == 0.0f);
    }
    
    SECTION("Size(float width, float height)")
    {
        pTK::Size size{cWidth, cHeight};
        
        REQUIRE(size.getWidth() == cWidth);
        REQUIRE(size.getHeight() == cHeight);
    }
}

TEST_CASE("Getters and Setters")
{
    // Testing Getters and Setters.
    float cWidth = 55.0f;
    float cHeight = 350.0f;
    
    SECTION("SetWidth(float width)")
    {
        pTK::Size size;
        size.setWidth(cWidth);
        REQUIRE(size.getWidth() == cWidth);
    }
    
    SECTION("SetHeight(float height)")
    {
        pTK::Size size;
        size.setHeight(cHeight);
        REQUIRE(size.getHeight() == cHeight);
    }
    
    SECTION("setSize(float width, float height)")
    {
        pTK::Size size;
        size.setSize(cWidth, cHeight);
        REQUIRE(size.getWidth() == cWidth);
        REQUIRE(size.getHeight() == cHeight);
    }
    
    SECTION("setSize(const Size& size)")
    {
        pTK::Size size{cWidth, cHeight};
        REQUIRE(size.getWidth() == cWidth);
        REQUIRE(size.getHeight() == cHeight);
    }
}

TEST_CASE("Copy and Assignment")
{
    // Testing Rectangle Copy and Assignment.
    float cWidth = 949.0f;
    float cHeight = 84.0f;
    
    pTK::Size size;
    size.setSize(cWidth, cHeight);
    
    SECTION("Copy")
    {
        pTK::Size tmp = size;
        REQUIRE(tmp.getWidth() == cWidth);
        REQUIRE(tmp.getHeight() == cHeight);
    }
    
    SECTION("Assignment")
    {
        pTK::Size tmp;
        tmp = size;
        REQUIRE(tmp.getWidth() == cWidth);
        REQUIRE(tmp.getHeight() == cHeight);
    }
}

TEST_CASE ("Comparison")
{
    // Testing Transformable Comparison.
    float cWidth = 57.0f;
    float cHeight = 384.0f;
    
    pTK::Size size;
    size.setSize(cWidth, cHeight);
    
    pTK::Size s1 = size;
    
    pTK::Size s2;
    s2.setHeight(cHeight);
    
    pTK::Size s3;
    s3.setWidth(cWidth);
    
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


