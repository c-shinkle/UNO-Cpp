#include <iostream>
#include "uno/GameLib.h"
#include "uno/Card.h"

void gameLoop()
{
    std::vector<Card> cards = { 
        {Card::Color::Red, Card::Value::Zero},
        {Card::Color::Red, Card::Value::One},
        {Card::Color::Red, Card::Value::Two},
    };
    std::string* hand = printHand(cards);
    std::cout << *hand;
}

std::string* printHand(std::vector<Card>& cards)
{
    std::string tops_and_bottoms(" ");

    for (int i = 0; i < cards.size(); ++i)
        tops_and_bottoms += "- ";

    tops_and_bottoms.push_back('\n');

    std::string* hand = new std::string();
    hand->append(tops_and_bottoms);
    for(Card card: cards)
    {
        hand->push_back('|');
        hand->push_back(static_cast<char>(card.m_eValue) + 48);
    }
    hand->append("|\n");
    hand->append(tops_and_bottoms);
    return hand;
}
