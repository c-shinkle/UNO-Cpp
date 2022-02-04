#include <iostream>
#include "uno/GameLib.h"

// Hide the console window.
#ifdef _WIN32
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

int main()
{
    //
    // Seed random number generator.
    std::srand((unsigned int)std::time(0));

    //FeatureTest(); // Comment out if desired.
    //KeyInputTest();
    OpenGLTest();
    //gameLoop();
}
