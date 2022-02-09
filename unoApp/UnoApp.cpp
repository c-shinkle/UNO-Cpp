#include <iostream>
#include "uno/GameLib.h"
#include "uno/Game.h"

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
    //gameLoop();

    Game demo;
    if (demo.Construct(1280, 720, 1, 1))
        //if (demo.Construct(320, 180, 4, 4))
        demo.Start();
    return 0;
}
