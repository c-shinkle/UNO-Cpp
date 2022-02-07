#include <iostream>
#include "uno/GameLib.h"
#include "uno/Example.h"

int main()
{
    //
    // Seed random number generator.
    std::srand((unsigned int)std::time(0));

    //FeatureTest(); // Comment out if desired.
    //KeyInputTest();
    //gameLoop();

    Example demo;
    if (demo.Construct(1280, 720, 1, 1))
        //if (demo.Construct(320, 180, 4, 4))
        demo.Start();
    return 0;
}
