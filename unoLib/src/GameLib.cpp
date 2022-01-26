#include <iostream>
#include <string>
#include "uno/GameLib.h"
#include "uno/Hand.h"

void gameLoop()
{
    Hand Deck;
    //Hand childHand = Hand(3, Deck);
    //std::string* hand = Deck.GetHandString();
    std::string* hand = Deck.GetHandString();
    std::cout << *hand;
}
