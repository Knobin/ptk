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
        REQUIRE(size.getMinSize() == pTK::Size::Min);
        REQUIRE(size.getSize() == ZERO);
        REQUIRE(size.getMaxSize() == pTK::Size::Max);
    }
    
    SECTION("Sizable(const Size& size)")
    {
        pTK::Size s1(10, 20);
        pTK::Sizable size(s1);
        REQUIRE(size.getMinSize() == pTK::Size::Min);
        REQUIRE(size.getSize() == pTK::Size(10, 20));
        REQUIRE(size.getMaxSize() == pTK::Size::Max);
        
        size.setMaxSize(pTK::Size(50, 100));
        REQUIRE(size.getMinSize() == pTK::Size::Min);
        REQUIRE(size.getSize() == pTK::Size(10, 20));
        REQUIRE(size.getMaxSize() == pTK::Size(50, 100));
    }
}

TEST_CASE("Minimal Size")
{
    // Testing the behaviour of the setMinSize function.
    // The maximum size is not tested here and is by default set to pTK::Size::Max.

    SECTION("Blank Size")
    {
        pTK::Sizable obj;
        pTK::Size minSize(10, 2);
        obj.setMinSize(minSize);

        REQUIRE(obj.getMinSize() == minSize);
        REQUIRE(obj.getSize() == minSize);
    }

    SECTION("minimal size is smaller than size")
    {
        pTK::Sizable obj;
        pTK::Size size{100, 200};
        obj.setSize(size);

        // Only width is smaller.
        pTK::Size ms1{25, 200};
        obj.setMinSize(ms1);
        REQUIRE(obj.getMinSize() == pTK::Size{ms1.width, size.height});
        REQUIRE(obj.getSize() == size);

        // Only height is smaller.
        pTK::Size ms2{ms1.width, 50};
        obj.setMinSize(ms2);
        REQUIRE(obj.getMinSize() == ms2);
        REQUIRE(obj.getSize() == size);

        // Both are smaller than size and larger than current minimal size (25, 50).
        pTK::Size ms3{90, 190};
        obj.setMinSize(ms3);
        REQUIRE(obj.getMinSize() == ms3);
        REQUIRE(obj.getSize() == size);
    }

    SECTION("minimal size is larger than size")
    {
        pTK::Sizable obj;
        pTK::Size size{100, 200};
        obj.setSize(size);

        // Only width is larger.
        pTK::Size ms1{125, 200};
        obj.setMinSize(ms1);
        REQUIRE(obj.getMinSize() == ms1);
        REQUIRE(obj.getSize() == ms1);

        // Only height is larger.
        pTK::Size ms2{ms1.width, 250};
        obj.setMinSize(ms2);
        REQUIRE(obj.getMinSize() == ms2);
        REQUIRE(obj.getSize() == ms2);

        // Both are larger than size.
        pTK::Size ms3{300, 400};
        obj.setMinSize(ms3);
        REQUIRE(obj.getMinSize() == ms3);
        REQUIRE(obj.getSize() == ms3);
    }
}

TEST_CASE("Maximum Size")
{
    // Testing the behaviour of the setMaxSize function.
    // The mininum size is not tested here and is by default set to pTK::Size::Min.

    SECTION("Blank Size")
    {
        pTK::Sizable obj;
        pTK::Size maxSize(100, 200);
        obj.setMaxSize(maxSize);

        REQUIRE(obj.getMinSize() == ZERO);
        REQUIRE(obj.getSize() == ZERO);
        REQUIRE(obj.getMaxSize() == maxSize);
    }

    SECTION("maximum size is smaller than size")
    {
        pTK::Sizable obj;
        pTK::Size size{100, 200};
        obj.setSize(size);

        // Only width is smaller.
        pTK::Size ms1{25, 200};
        obj.setMaxSize(ms1);
        REQUIRE(obj.getSize() == ms1);
        REQUIRE(obj.getMaxSize() == ms1);

        // Only height is smaller.
        pTK::Size ms2{ms1.width, 50};
        obj.setMaxSize(ms2);
        REQUIRE(obj.getSize() == ms2);
        REQUIRE(obj.getMaxSize() == ms2);

        // Both are smaller than size and current maximum size (25, 50).
        pTK::Size ms3{10, 20};
        obj.setMaxSize(ms3);
        REQUIRE(obj.getSize() == ms3);
        REQUIRE(obj.getMaxSize() == ms3);
    }

    SECTION("maximum size is larger than size")
    {
        pTK::Sizable obj;
        pTK::Size size{100, 200};
        obj.setSize(size);

        // Only width is larger.
        pTK::Size ms1{125, 200};
        obj.setMaxSize(ms1);
        REQUIRE(obj.getSize() == size);
        REQUIRE(obj.getMaxSize() == ms1);

        // Only height is larger.
        pTK::Size ms2{ms1.width, 250};
        obj.setMaxSize(ms2);
        REQUIRE(obj.getSize() == size);
        REQUIRE(obj.getMaxSize() == ms2);

        // Both are larger than size.
        pTK::Size ms3{300, 400};
        obj.setMaxSize(ms3);
        REQUIRE(obj.getSize() == size);
        REQUIRE(obj.getMaxSize() == ms3);
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

        REQUIRE(tmp.getMinSize() == s1);
        REQUIRE(tmp.getSize() == s2);
        REQUIRE(tmp.getMaxSize() == s3);
    }
    
    SECTION("Assignment")
    {
        pTK::Sizable tmp;
        tmp = size;
        
        REQUIRE(size.getMinSize() == s1);
        REQUIRE(size.getSize() == s2);
        REQUIRE(size.getMaxSize() == s3);

        REQUIRE(tmp.getMinSize() == s1);
        REQUIRE(tmp.getSize() == s2);
        REQUIRE(tmp.getMaxSize() == s3);
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


