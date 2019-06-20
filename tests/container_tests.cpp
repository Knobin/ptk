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
    
    SECTION("add(const std::shared_ptr<Widget>& widget, int32_t priority)")
    {
        cont.add(widget, 10);
        REQUIRE(cont.get_priority(widget) == 10);
    }
    
    SECTION("Duplicates on add")
    {
        // cont already contains widget, add it again.
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
    
    SECTION("widget and priority")
    {
        int32_t priorities[3] = {30, 20, 10};
        cont.set_priority(widgets[0], priorities[0]);
        cont.set_priority(widgets[1], priorities[1]);
        cont.set_priority(widgets[2], priorities[2]);
        
        int i = 0;
        cont.for_each([&](const std::pair<std::shared_ptr<pTK::Widget>, int32_t>& pair){
            REQUIRE(pair.first == widgets[i]);
            REQUIRE(pair.second == priorities[i]);
            i++;
        });
    }
}

TEST_CASE("Container sort")
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
    
    SECTION("No sort required")
    {
        int32_t priorities[3] = {30, 20, 10};
        cont.set_priority(widgets[0], priorities[0]);
        cont.set_priority(widgets[1], priorities[1]);
        cont.set_priority(widgets[2], priorities[2]);
        
        int i = 0;
        cont.for_each([&](const std::pair<std::shared_ptr<pTK::Widget>, int32_t>& pair){
            REQUIRE(pair.first == widgets[i]);
            REQUIRE(pair.second == priorities[i]);
            i++;
        });
    }
    
    SECTION("Case 1")
    {
        int32_t priorities[3] = {20, 30, 10};
        cont.set_priority(widgets[0], priorities[0]);
        cont.set_priority(widgets[1], priorities[1]);
        cont.set_priority(widgets[2], priorities[2]);
        
        int32_t correct_order[3] = {1, 0, 2};
        
        int i = 0;
        cont.for_each([&](const std::pair<std::shared_ptr<pTK::Widget>, int32_t>& pair){
            REQUIRE(pair.first == widgets[correct_order[i]]);
            REQUIRE(pair.second == priorities[correct_order[i]]);
            i++;
        });
    }
    
    SECTION("Case 2")
    {
        int32_t priorities[3] = {10, 20, 30};
        cont.set_priority(widgets[0], priorities[0]);
        cont.set_priority(widgets[1], priorities[1]);
        cont.set_priority(widgets[2], priorities[2]);
        
        int32_t correct_order[3] = {2, 1, 0};
        
        int i = 0;
        cont.for_each([&](const std::pair<std::shared_ptr<pTK::Widget>, int32_t>& pair){
            REQUIRE(pair.first == widgets[correct_order[i]]);
            REQUIRE(pair.second == priorities[correct_order[i]]);
            i++;
        });
    }
    
    SECTION("Case 3")
    {
        int32_t priorities[3] = {30, 10, 20};
        cont.set_priority(widgets[0], priorities[0]);
        cont.set_priority(widgets[1], priorities[1]);
        cont.set_priority(widgets[2], priorities[2]);
        
        int32_t correct_order[3] = {0, 2, 1};
        
        int i = 0;
        cont.for_each([&](const std::pair<std::shared_ptr<pTK::Widget>, int32_t>& pair){
            REQUIRE(pair.first == widgets[correct_order[i]]);
            REQUIRE(pair.second == priorities[correct_order[i]]);
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
    
    int32_t priorities[3] = {30, 20, 10};
    cont.set_priority(widgets[0], priorities[0]);
    cont.set_priority(widgets[1], priorities[1]);
    cont.set_priority(widgets[2], priorities[2]);
    
    SECTION("Copy")
    {
        pTK::Container tmp = cont;

        int i = 0;
        bool equal = true;
        tmp.for_each([&](const std::pair<std::shared_ptr<pTK::Widget>, int32_t>& pair){
            if ((widgets[i] != pair.first) || (priorities[i] != pair.second))
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
        tmp.for_each([&](const std::pair<std::shared_ptr<pTK::Widget>, int32_t>& pair){
            if ((widgets[i] != pair.first) || (priorities[i] != pair.second))
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
    
    int32_t priorities[3] = {30, 20, 10};
    cont.set_priority(widgets[0], priorities[0]);
    cont.set_priority(widgets[1], priorities[1]);
    cont.set_priority(widgets[2], priorities[2]);
    
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

