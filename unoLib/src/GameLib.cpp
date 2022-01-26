#include <iostream>
#include <string>
#include "uno/GameLib.h"
#include "uno/Hand.h"

void gameLoop()
{
    Hand Deck;
    //
    // Before shuffling.
    std::string* hand = Deck.GetHandString();
    std::cout << *hand;
    //
    // After shuffling.
    Deck.Shuffle();
    std::string* hand2 = Deck.GetHandString();
    std::cout << *hand2;
}
