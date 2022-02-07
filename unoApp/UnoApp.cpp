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
    if (demo.Construct(256, 240, 4, 4))
        demo.Start();
}
