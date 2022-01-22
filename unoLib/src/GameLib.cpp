#include <iostream>
#include "uno/GameLib.h"

void gameLoop()
{
    std::vector<char> cards = {'1', '2', '3'};
    std::string* hand = printHand(cards);
    std::cout << *hand << std::endl;
}

std::string* printHand(std::vector<char>& cards)
{
    std::string* hand = new std::string();
    hand->append("My hand\n");
    hand->append(" - - -\n");
    for(char card: cards)
    {
        hand->push_back('|');
        hand->push_back(card);
    }
    hand->append("|\n");
    hand->append(" - - -\n");
    return hand;
}
