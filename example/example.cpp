#include <iostream>
#include <thread>

// settings
const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 540;

#include "ptk/application.hpp"

int main(int argc, char *argv[])
{
    pTK::Application app(argc, argv);
    pTK::Window window("pTK Test Environment", SCR_WIDTH, SCR_HEIGHT);

    return app.exec(&window);
}