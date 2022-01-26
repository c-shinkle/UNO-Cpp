#include <iostream>
#include "uno/GameLib.h"

int main()
{
    //
    // Seed random number generator.
    std::srand(std::time(0));
    gameLoop();
}
