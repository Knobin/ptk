//
//  example/image/ImageExample.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-06-22.
//

// pTK Headers
#include "ptk/Application.hpp"
#include "ptk/Window.hpp"
#include "ptk/util/ColorType.hpp"
#include "ptk/util/Pixmap.hpp"
#include "ptk/widgets/Image.hpp"

// C++ Headers
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>

// Divides the pixels in the pixmap into cubes and assigns a color to that cube.
static void GeneratePixmap(pTK::Pixmap& pixmap)
{
    static std::random_device dev;
    static std::mt19937 gen(dev());
    std::uniform_int_distribution<uint32_t> dist(0, std::numeric_limits<uint32_t>::max());

    // Loop through Pixmap and divide into cubes.
    for (std::ptrdiff_t y{0}; y < static_cast<std::ptrdiff_t>(pixmap.height()); y += 32)
    {
        for (std::ptrdiff_t x{0}; x < static_cast<std::ptrdiff_t>(pixmap.width()); x += 32)
        {
            const uint32_t hex = dist(gen);

            // Loop through pixels in cube.
            for (std::ptrdiff_t dy{0}; dy < 32; ++dy)
            {
                for (std::ptrdiff_t dx{0}; dx < 32; ++dx)
                {
                    uint8_t* bytes{pixmap.at(x + dx, y + dy)};
                    std::memcpy(bytes, reinterpret_cast<const uint8_t*>(&hex), 3);
                    bytes[3] = 0xFF;
                }
            }
        }
    }
}

int main(int argc, char* argv[])
{
    // Application and window.
    pTK::Application app("Image Example Application", argc, argv);
    pTK::Window window("Image Example Window", {960, 540});

    // Create a pixel buffer and set raw pixels
    pTK::Pixmap pixmap{32 * 12, 32 * 12, pTK::ColorType::RGBA_8888};
    GeneratePixmap(pixmap);

    // Create Image from Pixmap data.
    std::shared_ptr<pTK::Image> image{std::make_shared<pTK::Image>(pixmap)};

    // Check if data is valid and can be displayed.
    if (image->isValid())
        std::cout << "Image loaded successfully from Pixmap!" << std::endl;
    else
        std::cout << "Could not load image from Pixmap!" << std::endl;

    // Add image to window.
    window.add(image);

    // Initialization is done! Run the application.
    window.show();
    return app.exec(&window);
}
