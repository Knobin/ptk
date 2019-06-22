#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "ptk/core/container.hpp"
#include "ptk/core/widget.hpp"

TEST_CASE("Container Constructors")
{
    // Testing Constructors with correct data.
    
    SECTION("Container()")
    {
        pTK::Container cont;
        REQUIRE(cont.count() == 0);
    }
}

TEST_CASE("Container Add and Remove")
{
    // Testing Add and Remove.
    pTK::Container cont;
    std::shared_ptr<pTK::Widget> widget = std::make_shared<pTK::Widget>();
    
    SECTION("add(const std::shared_ptr<Widget>& widget)")
    {
        cont.add(widget);
        REQUIRE(cont.count() == 1);
    }
    
    SECTION("remove(const std::shared_ptr<Widget>& widget)")
    {
        cont.remove(widget);
        REQUIRE(cont.count() == 0);
    }
    
    SECTION("Duplicates on add")
    {
        cont.add(widget);
        cont.add(widget);
        
        // Container does not allow duplicates.
        REQUIRE(cont.count() == 1);
    }
    
}

TEST_CASE("Container for_each")
{
    // Testing for_each functions.
    pTK::Container cont;
    std::shared_ptr<pTK::Widget> widgets[3];
    
    widgets[0] = std::make_shared<pTK::Widget>();
    widgets[1] = std::make_shared<pTK::Widget>();
    widgets[2] = std::make_shared<pTK::Widget>();
    
    cont.add(widgets[0]);
    cont.add(widgets[1]);
    cont.add(widgets[2]);
    
    SECTION("widget")
    {
        int i = 0;
        cont.for_each([&](const std::shared_ptr<pTK::Widget>& widget){
            REQUIRE(widget == widgets[i]);
            i++;
        });
    }
}

TEST_CASE("Container Copy and Assignment")
{
    // Testing Container Copy and Assignment.
    pTK::Container cont;
    std::shared_ptr<pTK::Widget> widgets[3];
    
    widgets[0] = std::make_shared<pTK::Widget>();
    widgets[1] = std::make_shared<pTK::Widget>();
    widgets[2] = std::make_shared<pTK::Widget>();
    
    cont.add(widgets[0]);
    cont.add(widgets[1]);
    cont.add(widgets[2]);
    
    SECTION("Copy")
    {
        pTK::Container tmp = cont;

        int i = 0;
        bool equal = true;
        tmp.for_each([&](const std::shared_ptr<pTK::Widget>& widget){
            if (widgets[i] != widget)
                equal = false;
            i++;
        });
        
        REQUIRE(tmp.count() == cont.count());
        REQUIRE(i == cont.count());
        REQUIRE(equal == true);
    }
    
    SECTION("Assignment")
    {
        pTK::Container tmp;
        tmp = cont;
        
        int i = 0;
        bool equal = true;
        tmp.for_each([&](const std::shared_ptr<pTK::Widget>& widget){
            if (widgets[i] != widget)
                equal = false;
            i++;
        });
        
        REQUIRE(tmp.count() == cont.count());
        REQUIRE(i == cont.count());
        REQUIRE(equal == true);
    }
}

TEST_CASE ("Container Comparison")
{
    // Testing Transformable Comparison.
    pTK::Container cont;
    std::shared_ptr<pTK::Widget> widgets[3];
    
    widgets[0] = std::make_shared<pTK::Widget>();
    widgets[1] = std::make_shared<pTK::Widget>();
    widgets[2] = std::make_shared<pTK::Widget>();
    
    cont.add(widgets[0]);
    cont.add(widgets[1]);
    cont.add(widgets[2]);
    
    pTK::Container tmp = cont;
    pTK::Container tmp2;
    pTK::Container tmp3;
    tmp3.add(widgets[0]);
    pTK::Container tmp4;
    tmp4.add(widgets[0]);
    
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

