#include <iostream>
#include "uno/GameLib.h"

int main()
{
    //
    // Seed random number generator.
    std::srand((unsigned int)std::time(0));

    //FeatureTest(); // Comment out if desired.
    //KeyInputTest();
    gameLoop();
}
