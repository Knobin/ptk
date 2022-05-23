// Catch2 Headers
#include "catch2/catch_test_macros.hpp"

// pTK Headers
#include "ptk/util/Color.hpp"

// C++ Headers
#include <memory>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

struct s_color
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
    uint32_t all;
};

std::vector<std::pair<s_color, std::string>> data{
    std::make_pair<s_color, std::string>({255,  255,  255,  255, 0xFFFFFFFF}, "All High"),
    std::make_pair<s_color, std::string>({255,    0,    0,    0, 0xFF000000}, "Only R"),
    std::make_pair<s_color, std::string>({  0,  255,    0,    0, 0x00FF0000}, "Only G"),
    std::make_pair<s_color, std::string>({  0,    0,  255,    0, 0x0000FF00}, "Only B"),
    std::make_pair<s_color, std::string>({  0,    0,    0,  255, 0x000000FF}, "Only A"),
    
    std::make_pair<s_color, std::string>({255,  255,    0,    0, 0xFFFF0000}, "R and G"),
    std::make_pair<s_color, std::string>({255,    0,  255,    0, 0xFF00FF00}, "R and B"),
    std::make_pair<s_color, std::string>({255,    0,    0,  255, 0xFF0000FF}, "R and A"),
    std::make_pair<s_color, std::string>({  0,  255,  255,    0, 0x00FFFF00}, "G and B"),
    std::make_pair<s_color, std::string>({  0,  255,    0,  255, 0x00FF00FF}, "G and A"),
    std::make_pair<s_color, std::string>({  0,    0,  255,  255, 0x0000FFFF}, "B and A"),
    
    std::make_pair<s_color, std::string>({  0,  255,  255,  255, 0x00FFFFFF}, "G, B and A"),
    std::make_pair<s_color, std::string>({255,    0,  255,  255, 0xFF00FFFF}, "R, B and A"),
    std::make_pair<s_color, std::string>({255,  255,    0,  255, 0xFFFF00FF}, "R, G and A"),
    std::make_pair<s_color, std::string>({255,  255,  255,    0, 0xFFFFFF00}, "R, G and B"),
    
    std::make_pair<s_color, std::string>({ 58,  249,  221,   67, 0x3AF9DD43}, "Random: 0x3AF9DD43"),
    std::make_pair<s_color, std::string>({111,    9,  168,   24, 0x6F09A818}, "Random: 0x6F09A818"),
    std::make_pair<s_color, std::string>({225,   74,  160,   26, 0xE14AA01A}, "Random: 0xE14AA01A"),
    std::make_pair<s_color, std::string>({176,  207,   72,   58, 0xB0CF483A}, "Random: 0xB0CF483A"),
    std::make_pair<s_color, std::string>({ 26,   51,    9,    2, 0x1A330902}, "Random: 0x1A330902"),
    std::make_pair<s_color, std::string>({113,   32,  167,   92, 0x7120A75C}, "Random: 0x7120A75C"),
    std::make_pair<s_color, std::string>({237,  145,  160,   11, 0xED91A00B}, "Random: 0xED91A00B"),
    std::make_pair<s_color, std::string>({113,   62,  100,   21, 0x713E6415}, "Random: 0x713E6415"),
    std::make_pair<s_color, std::string>({225,  142,  133,  221, 0xE18E85DD}, "Random: 0xE18E85DD"),
    std::make_pair<s_color, std::string>({ 16,  128,   73,   44, 0x1080492C}, "Random: 0x1080492C"),
    std::make_pair<s_color, std::string>({ 44,  125,  170,   91, 0x2C7DAA5B}, "Random: 0x2C7DAA5B"),
    std::make_pair<s_color, std::string>({ 72,  122,  237,  184, 0x487AEDB8}, "Random: 0x487AEDB8"),
    std::make_pair<s_color, std::string>({187,  103,   12,   71, 0xBB670C47}, "Random: 0xBB670C47"),
    std::make_pair<s_color, std::string>({ 22,  210,  158,   92, 0x16D29E5C}, "Random: 0x16D29E5C"),
    std::make_pair<s_color, std::string>({242,  113,  161,   59, 0xF271A13B}, "Random: 0xF271A13B"),
    std::make_pair<s_color, std::string>({222,  181,  115,  195, 0xDEB573C3}, "Random: 0xDEB573C3"),
    std::make_pair<s_color, std::string>({220,  174,  176,  224, 0xDCAEB0E0}, "Random: 0xDCAEB0E0"),
    std::make_pair<s_color, std::string>({106,  100,   90,  150, 0x6A645A96}, "Random: 0x6A645A96"),
    std::make_pair<s_color, std::string>({ 63,   45,   56,  227, 0x3F2D38E3}, "Random: 0x3F2D38E3"),
    std::make_pair<s_color, std::string>({111,  199,  250,  149, 0x6FC7FA95}, "Random: 0x6FC7FA95"),
    std::make_pair<s_color, std::string>({232,   65,   87,  215, 0xE84157D7}, "Random: 0xE84157D7"),
    std::make_pair<s_color, std::string>({197,  154,  119,  190, 0xC59A77BE}, "Random: 0xC59A77BE"),
    std::make_pair<s_color, std::string>({ 10,  166,  204,  248, 0x0AA6CCF8}, "Random: 0x0AA6CCF8"),
    std::make_pair<s_color, std::string>({156,  195,  198,  141, 0x9CC3C68D}, "Random: 0x9CC3C68D"),
    std::make_pair<s_color, std::string>({165,   69,   85,  114, 0xA5455572}, "Random: 0xA5455572"),
    std::make_pair<s_color, std::string>({135,  143,  214,  163, 0x878FD6A3}, "Random: 0x878FD6A3"),
    std::make_pair<s_color, std::string>({ 68,  145,  102,  218, 0x449166DA}, "Random: 0x449166DA"),
    std::make_pair<s_color, std::string>({ 74,  247,   43,  249, 0x4AF72BF9}, "Random: 0x4AF72BF9"),
    std::make_pair<s_color, std::string>({136,   98,  254,  181, 0x8862FEB5}, "Random: 0x8862FEB5"),
    std::make_pair<s_color, std::string>({ 42,   89,  143,  216, 0x2A598FD8}, "Random: 0x2A598FD8"),
    std::make_pair<s_color, std::string>({234,  133,  186,   95, 0xEA85BA5F}, "Random: 0xEA85BA5F"),
    std::make_pair<s_color, std::string>({ 21,  249,  111,  228, 0x15F96FE4}, "Random: 0x15F96FE4"),
    std::make_pair<s_color, std::string>({ 42,  102,   48,  211, 0x2A6630D3}, "Random: 0x2A6630D3"),
    std::make_pair<s_color, std::string>({126,  243,  202,  245, 0x7EF3CAF5}, "Random: 0x7EF3CAF5"),
    std::make_pair<s_color, std::string>({ 83,  203,  217,  155, 0x53CBD99B}, "Random: 0x53CBD99B"),
    std::make_pair<s_color, std::string>({229,  177,   80,  174, 0xE5B150AE}, "Random: 0xE5B150AE"),
    std::make_pair<s_color, std::string>({254,   31,  226,  142, 0xFE1FE28E}, "Random: 0xFE1FE28E"),
    std::make_pair<s_color, std::string>({ 84,  234,  220,  190, 0x54EADCBE}, "Random: 0x54EADCBE"),
    std::make_pair<s_color, std::string>({211,   78,   88,   21, 0xD34E5815}, "Random: 0xD34E5815"),
    std::make_pair<s_color, std::string>({191,   77,  249,   70, 0xBF4DF946}, "Random: 0xBF4DF946"),
    std::make_pair<s_color, std::string>({ 99,  126,  230,  173, 0x637EE6AD}, "Random: 0x637EE6AD"),
    std::make_pair<s_color, std::string>({120,  225,  193,   62, 0x78E1C13E}, "Random: 0x78E1C13E"),
    std::make_pair<s_color, std::string>({ 84,  179,  121,  214, 0x54B379D6}, "Random: 0x54B379D6"),
    std::make_pair<s_color, std::string>({224,  198,  204,   87, 0xE0C6CC57}, "Random: 0xE0C6CC57"),
    std::make_pair<s_color, std::string>({ 78,  149,  183,    0, 0x4E95B700}, "Random: 0x4E95B700"),
    std::make_pair<s_color, std::string>({204,  236,  217,   33, 0xCCECD921}, "Random: 0xCCECD921"),
    std::make_pair<s_color, std::string>({ 19,  239,   84,  201, 0x13EF54C9}, "Random: 0x13EF54C9"),
    std::make_pair<s_color, std::string>({203,  232,  177,  113, 0xCBE8B171}, "Random: 0xCBE8B171"),
    std::make_pair<s_color, std::string>({249,   54,   95,  128, 0xF9365F80}, "Random: 0xF9365F80"),
    std::make_pair<s_color, std::string>({ 35,   21,  180,   97, 0x2315B461}, "Random: 0x2315B461"),
    std::make_pair<s_color, std::string>({213,   38,  250,   31, 0xD526FA1F}, "Random: 0xD526FA1F"),
    std::make_pair<s_color, std::string>({ 15,   50,  237,  251, 0x0F32EDFB}, "Random: 0x0F32EDFB"),
    std::make_pair<s_color, std::string>({217,  225,   13,   68, 0xD9E10D44}, "Random: 0xD9E10D44"),
    std::make_pair<s_color, std::string>({ 94,  118,    0,  211, 0x5E7600D3}, "Random: 0x5E7600D3"),
    std::make_pair<s_color, std::string>({ 96,   12,   13,  227, 0x600C0DE3}, "Random: 0x600C0DE3"),
    std::make_pair<s_color, std::string>({ 35,   60,   92,  248, 0x233C5CF8}, "Random: 0x233C5CF8"),
    std::make_pair<s_color, std::string>({213,   33,  126,  145, 0xD5217E91}, "Random: 0xD5217E91"),
    std::make_pair<s_color, std::string>({224,  147,   77,  220, 0xE0934DDC}, "Random: 0xE0934DDC"),
    std::make_pair<s_color, std::string>({160,  243,  253,  192, 0xA0F3FDC0}, "Random: 0xA0F3FDC0"),
    std::make_pair<s_color, std::string>({ 39,  212,   77,  206, 0x27D44DCE}, "Random: 0x27D44DCE"),
    std::make_pair<s_color, std::string>({ 13,  167,  227,  241, 0x0DA7E3F1}, "Random: 0x0DA7E3F1"),
    std::make_pair<s_color, std::string>({200,  127,   47,  146, 0xC87F2F92}, "Random: 0xC87F2F92"),
    std::make_pair<s_color, std::string>({200,  117,  240,  128, 0xC875F080}, "Random: 0xC875F080"),
    std::make_pair<s_color, std::string>({159,   91,    1,   32, 0x9F5B0120}, "Random: 0x9F5B0120"),
    std::make_pair<s_color, std::string>({170,  185,   19,  224, 0xAAB913E0}, "Random: 0xAAB913E0"),
    std::make_pair<s_color, std::string>({ 27,  136,   55,  202, 0x1B8837CA}, "Random: 0x1B8837CA"),
    std::make_pair<s_color, std::string>({193,  188,  185,  127, 0xC1BCB97F}, "Random: 0xC1BCB97F"),
    std::make_pair<s_color, std::string>({ 44,  218,  123,  164, 0x2CDA7BA4}, "Random: 0x2CDA7BA4"),
    std::make_pair<s_color, std::string>({ 42,  153,  221,    7, 0x2A99DD07}, "Random: 0x2A99DD07"),
    std::make_pair<s_color, std::string>({247,  130,  182,  249, 0xF782B6F9}, "Random: 0xF782B6F9"),
    std::make_pair<s_color, std::string>({ 43,   82,  224,  178, 0x2B52E0B2}, "Random: 0x2B52E0B2"),
    std::make_pair<s_color, std::string>({235,  226,  182,  126, 0xEBE2B67E}, "Random: 0xEBE2B67E"),
    std::make_pair<s_color, std::string>({137,   45,    7,   81, 0x892D0751}, "Random: 0x892D0751"),
    std::make_pair<s_color, std::string>({149,   57,  255,  237, 0x9539FFED}, "Random: 0x9539FFED"),
    std::make_pair<s_color, std::string>({ 57,  246,  225,  242, 0x39F6E1F2}, "Random: 0x39F6E1F2"),
    std::make_pair<s_color, std::string>({ 97,  123,   46,  123, 0x617B2E7B}, "Random: 0x617B2E7B"),
    std::make_pair<s_color, std::string>({ 13,   42,   66,  189, 0x0D2A42BD}, "Random: 0x0D2A42BD"),
    std::make_pair<s_color, std::string>({109,  183,   51,    5, 0x6DB73305}, "Random: 0x6DB73305"),
    std::make_pair<s_color, std::string>({252,   24,    4,  196, 0xFC1804C4}, "Random: 0xFC1804C4"),
    std::make_pair<s_color, std::string>({ 49,  147,  122,   93, 0x31937A5D}, "Random: 0x31937A5D"),
    std::make_pair<s_color, std::string>({ 80,  221,   47,  206, 0x50DD2FCE}, "Random: 0x50DD2FCE"),
    std::make_pair<s_color, std::string>({ 64,  133,  109,  231, 0x40856DE7}, "Random: 0x40856DE7"),
    std::make_pair<s_color, std::string>({155,  135,  238,   64, 0x9B87EE40}, "Random: 0x9B87EE40"),
    std::make_pair<s_color, std::string>({ 50,  156,  204,   38, 0x329CCC26}, "Random: 0x329CCC26"),
    std::make_pair<s_color, std::string>({112,   99,  178,   49, 0x7063B231}, "Random: 0x7063B231"),
    std::make_pair<s_color, std::string>({152,  179,  248,  114, 0x98B3F872}, "Random: 0x98B3F872"),
    std::make_pair<s_color, std::string>({244,   65,   50,  202, 0xF44132CA}, "Random: 0xF44132CA"),
    std::make_pair<s_color, std::string>({233,  128,  114,  164, 0xE98072A4}, "Random: 0xE98072A4"),
    std::make_pair<s_color, std::string>({ 16,   23,    2,   29, 0x1017021D}, "Random: 0x1017021D"),
    std::make_pair<s_color, std::string>({103,   15,  157,  128, 0x670F9D80}, "Random: 0x670F9D80"),
    std::make_pair<s_color, std::string>({214,  240,  121,   92, 0xD6F0795C}, "Random: 0xD6F0795C"),
    std::make_pair<s_color, std::string>({208,  109,  237,  158, 0xD06DED9E}, "Random: 0xD06DED9E"),
    std::make_pair<s_color, std::string>({ 47,  108,  183,  112, 0x2F6CB770}, "Random: 0x2F6CB770"),
    std::make_pair<s_color, std::string>({119,  175,   95,   18, 0x77AF5F12}, "Random: 0x77AF5F12"),
    std::make_pair<s_color, std::string>({ 28,  108,  137,  177, 0x1C6C89B1}, "Random: 0x1C6C89B1"),
    std::make_pair<s_color, std::string>({107,  253,   52,  144, 0x6BFD3490}, "Random: 0x6BFD3490"),
    std::make_pair<s_color, std::string>({ 55,   93,   69,   44, 0x375D452C}, "Random: 0x375D452C"),
    std::make_pair<s_color, std::string>({254,  229,   11,  133, 0xFEE50B85}, "Random: 0xFEE50B85"),
    std::make_pair<s_color, std::string>({242,  165,  196,  212, 0xF2A5C4D4}, "Random: 0xF2A5C4D4"),
    std::make_pair<s_color, std::string>({ 35,   59,    3,  214, 0x233B03D6}, "Random: 0x233B03D6")
};

s_color correct_add(const pTK::Color& lhs, const pTK::Color& rhs)
{
    s_color new_s_color;
    new_s_color.red     = ((lhs.r + rhs.r) < 256)  ?  (lhs.r + rhs.r) : 255;
    new_s_color.green   = ((lhs.g + rhs.g) < 256)  ?  (lhs.g + rhs.g) : 255;
    new_s_color.blue    = ((lhs.b + rhs.b) < 256)  ?  (lhs.b + rhs.b) : 255;
    new_s_color.alpha   = ((lhs.a + rhs.a) < 256)  ?  (lhs.a + rhs.a) : 255;
    
    new_s_color.all = 0;
    new_s_color.all |= (new_s_color.red << 24);
    new_s_color.all |= (new_s_color.green << 16);
    new_s_color.all |= (new_s_color.blue << 8);
    new_s_color.all |= new_s_color.alpha;
    
    return new_s_color;
}

s_color correct_sub(const pTK::Color& lhs, const pTK::Color& rhs)
{
    s_color new_s_color;
    new_s_color.red     = ((lhs.r - rhs.r) > 0)  ?  (lhs.r - rhs.r) : 0;
    new_s_color.green   = ((lhs.g - rhs.g) > 0)  ?  (lhs.g - rhs.g) : 0;
    new_s_color.blue    = ((lhs.b - rhs.b) > 0)  ?  (lhs.b - rhs.b) : 0;
    new_s_color.alpha   = ((lhs.a - rhs.a) > 0)  ?  (lhs.a - rhs.a) : 0;
    
    new_s_color.all = 0;
    new_s_color.all |= (new_s_color.red << 24);
    new_s_color.all |= (new_s_color.green << 16);
    new_s_color.all |= (new_s_color.blue << 8);
    new_s_color.all |= new_s_color.alpha;
    
    return new_s_color;
}

TEST_CASE ("Correct Color Constructors")
{
    // Testing Constructors with correct data.

    SECTION("Color()")
    {
        pTK::Color color;
        
        REQUIRE(color.r == 0);
        REQUIRE(color.g == 0);
        REQUIRE(color.b == 0);
        REQUIRE(color.a == 255);
        REQUIRE(color.to_int() == 0x000000FF);
    }
    
    for (const auto& p : data)
    {
        SECTION("Color(uint32_t color) " + p.second + " input: " + std::to_string(p.first.all))
        {
            pTK::Color color(p.first.all);
            REQUIRE(color.r == p.first.red);
            REQUIRE(color.g == p.first.green);
            REQUIRE(color.b == p.first.blue);
            REQUIRE(color.a == p.first.alpha);
            REQUIRE(color.to_int() == p.first.all);
        }
        
        SECTION("Color(uint8_t red, uint8_t green, uint8_t blue) " + p.second + " input: " + std::to_string(p.first.all))
        {
            pTK::Color color(p.first.red, p.first.green, p.first.blue);
            REQUIRE(color.r == p.first.red);
            REQUIRE(color.g == p.first.green);
            REQUIRE(color.b == p.first.blue);
            REQUIRE(color.a == 255);
            uint32_t new_all = p.first.all;
            new_all |= 0x000000FF;
            REQUIRE(color.to_int() == new_all);
        }
        
        SECTION("Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) " + p.second + " input: " + std::to_string(p.first.all))
        {
            pTK::Color color(p.first.red, p.first.green, p.first.blue, p.first.alpha);
            REQUIRE(color.r == p.first.red);
            REQUIRE(color.g == p.first.green);
            REQUIRE(color.b == p.first.blue);
            REQUIRE(color.a == p.first.alpha);
            REQUIRE(color.to_int() == p.first.all);
        }
        
        SECTION("Color(const Color& rhs) " + p.second + " input: " + std::to_string(p.first.all))
        {
            pTK::Color color(p.first.red, p.first.green, p.first.blue, p.first.alpha);
            pTK::Color copy = color;
            REQUIRE(copy.r == p.first.red);
            REQUIRE(copy.g == p.first.green);
            REQUIRE(copy.b == p.first.blue);
            REQUIRE(copy.a == p.first.alpha);
            REQUIRE(copy.to_int() == p.first.all);
        }
    }
}

TEST_CASE ("Color Get")
{
    // Testing Get.
    
    for (const auto& p : data)
    {
        SECTION("get_*() " + p.second + " input: " + std::to_string(p.first.all))
        {
            pTK::Color color(p.first.red, p.first.green, p.first.blue, p.first.alpha);
            REQUIRE(color.r == p.first.red);
            REQUIRE(color.g == p.first.green);
            REQUIRE(color.b == p.first.blue);
            REQUIRE(color.a == p.first.alpha);
            REQUIRE(color.to_int() == p.first.all);
        }
    }
}

TEST_CASE ("Color Set")
{
    // Testing Set.
    
    for (const auto& p : data)
    {
        SECTION("set variables " + p.second + " input: " + std::to_string(p.first.all))
        {
            pTK::Color color;
            color.r = p.first.red;
            color.g = p.first.green;
            color.b = p.first.blue;
            color.a = p.first.alpha;
            REQUIRE(color.r == p.first.red);
            REQUIRE(color.g == p.first.green);
            REQUIRE(color.b == p.first.blue);
            REQUIRE(color.a == p.first.alpha);
            REQUIRE(color.to_int() == p.first.all);
        }
        
        SECTION("set_rgb(uint8_t red, uint8_t green, uint8_t blue) " + p.second + " input: " + std::to_string(p.first.all))
        {
            pTK::Color color;
            color.set_rgb(p.first.red, p.first.green, p.first.blue);
            REQUIRE(color.r == p.first.red);
            REQUIRE(color.g == p.first.green);
            REQUIRE(color.b == p.first.blue);
            REQUIRE(color.a == 255);
            uint32_t new_all = p.first.all;
            new_all |= 0x000000FF;
            REQUIRE(color.to_int() == new_all);
        }
        
        SECTION("set_rgb(uint32_t color) " + p.second + " input: " + std::to_string(p.first.all))
        {
            pTK::Color color;
            color.set_rgb(p.first.all);
            REQUIRE(color.r == p.first.red);
            REQUIRE(color.g == p.first.green);
            REQUIRE(color.b == p.first.blue);
            REQUIRE(color.a == 255);
            uint32_t new_all = p.first.all;
            new_all |= 0x000000FF;
            REQUIRE(color.to_int() == new_all);
        }
        
        SECTION("set_rgba(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) " + p.second + " input: " + std::to_string(p.first.all))
        {
            pTK::Color color;
            color.set_rgba(p.first.red, p.first.green, p.first.blue, p.first.alpha);
            REQUIRE(color.r == p.first.red);
            REQUIRE(color.g == p.first.green);
            REQUIRE(color.b == p.first.blue);
            REQUIRE(color.a == p.first.alpha);
            REQUIRE(color.to_int() == p.first.all);
        }
        
        SECTION("set_rgba(uint32_t color) " + p.second + " input: " + std::to_string(p.first.all))
        {
            pTK::Color set_raw;
            set_raw.set_rgba(p.first.all);
            REQUIRE(set_raw.r == p.first.red);
            REQUIRE(set_raw.g == p.first.green);
            REQUIRE(set_raw.b == p.first.blue);
            REQUIRE(set_raw.a == p.first.alpha);
            REQUIRE(set_raw.to_int() == p.first.all);
        }
    }
}

TEST_CASE ("Color Operators")
{
    // Testing Operators.
    
    for (const auto& p : data)
    {
        SECTION("Assignment Operator" + p.second + " input: " + std::to_string(p.first.all))
        {
            pTK::Color color(p.first.red, p.first.green, p.first.blue, p.first.alpha);
            pTK::Color copy;
            copy = color;
            REQUIRE(copy.r == p.first.red);
            REQUIRE(copy.g == p.first.green);
            REQUIRE(copy.b == p.first.blue);
            REQUIRE(copy.a == p.first.alpha);
            REQUIRE(copy.to_int() == p.first.all);
        }
        
        SECTION("Operator+" + p.second + " input: " + std::to_string(p.first.all))
        {
            pTK::Color color(p.first.red, p.first.green, p.first.blue, p.first.alpha);
            auto at = std::find_if(data.begin(), data.end(), [&](const auto& rhs){
                return rhs.second == p.second;
            });
            for (auto it = at; it != data.end(); ++it)
            {
                pTK::Color copy;
                pTK::Color add(it->first.red, it->first.green, it->first.blue, it->first.alpha);
                copy = color + add;
                s_color c_add = correct_add(color, add);
                REQUIRE(copy.r == c_add.red);
                REQUIRE(copy.g == c_add.green);
                REQUIRE(copy.b == c_add.blue);
                REQUIRE(copy.a == c_add.alpha);
                REQUIRE(copy.to_int() == c_add.all);
            }
        }
        
        SECTION("Operator-" + p.second + " input: " + std::to_string(p.first.all))
        {
            pTK::Color color(p.first.red, p.first.green, p.first.blue, p.first.alpha);
            auto at = std::find_if(data.begin(), data.end(), [&](const auto& rhs){
                return rhs.second == p.second;
            });
            for (auto it = at; it != data.end(); ++it)
            {
                pTK::Color copy;
                pTK::Color sub(it->first.red, it->first.green, it->first.blue, it->first.alpha);
                copy = color - sub;
                s_color c_sub = correct_sub(color, sub);
                REQUIRE(copy.r == c_sub.red);
                REQUIRE(copy.g == c_sub.green);
                REQUIRE(copy.b == c_sub.blue);
                REQUIRE(copy.a == c_sub.alpha);
                REQUIRE(copy.to_int() == c_sub.all);
            }
        }
        
        SECTION("Operator+=" + p.second + " input: " + std::to_string(p.first.all))
        {
            pTK::Color color(p.first.red, p.first.green, p.first.blue, p.first.alpha);
            auto at = std::find_if(data.begin(), data.end(), [&](const auto& rhs){
                return rhs.second == p.second;
            });
            for (auto it = at; it != data.end(); ++it)
            {
                pTK::Color copy = color;
                pTK::Color add(it->first.red, it->first.green, it->first.blue, it->first.alpha);
                copy += add;
                s_color c_add = correct_add(color, add);
                REQUIRE(copy.r == c_add.red);
                REQUIRE(copy.g == c_add.green);
                REQUIRE(copy.b == c_add.blue);
                REQUIRE(copy.a == c_add.alpha);
                REQUIRE(copy.to_int() == c_add.all);
            }
        }
        
        SECTION("Operator-=" + p.second + " input: " + std::to_string(p.first.all))
        {
            pTK::Color color(p.first.red, p.first.green, p.first.blue, p.first.alpha);
            auto at = std::find_if(data.begin(), data.end(), [&](const auto& rhs){
                return rhs.second == p.second;
            });
            for (auto it = at; it != data.end(); ++it)
            {
                pTK::Color copy = color;
                pTK::Color sub(it->first.red, it->first.green, it->first.blue, it->first.alpha);
                copy -= sub;
                s_color c_sub = correct_sub(color, sub);
                REQUIRE(copy.r == c_sub.red);
                REQUIRE(copy.g == c_sub.green);
                REQUIRE(copy.b == c_sub.blue);
                REQUIRE(copy.a == c_sub.alpha);
                REQUIRE(copy.to_int() == c_sub.all);
            }
        }
        
        SECTION("Operator==" + p.second + " input: " + std::to_string(p.first.all))
        {
            pTK::Color first(p.first.red, p.first.green, p.first.blue, p.first.alpha);
            pTK::Color second(p.first.red, p.first.green, p.first.blue, p.first.alpha);
            REQUIRE(first == second);
            
            auto at = std::find_if(data.begin(), data.end(), [&](const auto& rhs){
                return rhs.second == p.second;
            });
            for (auto it = ++at; it != data.end(); ++it)
            {
                pTK::Color second2(it->first.red, it->first.green, it->first.blue, it->first.alpha);
                REQUIRE_FALSE(first == second2);
            }
        }
        
        SECTION("Operator!=" + p.second + " input: " + std::to_string(p.first.all))
        {
            pTK::Color first(p.first.red, p.first.green, p.first.blue, p.first.alpha);
            pTK::Color second(p.first.red, p.first.green, p.first.blue, p.first.alpha);
            REQUIRE_FALSE(first != second);
            
            auto at = std::find_if(data.begin(), data.end(), [&](const auto& rhs){
                return rhs.second == p.second;
            });
            for (auto it = ++at; it != data.end(); ++it)
            {
                pTK::Color second2(it->first.red, it->first.green, it->first.blue, it->first.alpha);
                REQUIRE(first != second2);
            }
        }
    }
}
