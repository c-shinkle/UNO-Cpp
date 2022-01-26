#include <iostream>
#include "uno/Hand.h"

Hand::Hand()
{
	// Default constructor creates the full deck.
	for (int eColor = Card::Color::Red; eColor <= Card::Color::Blue; ++eColor) {
		for (int eValue = Card::Value::Zero; eValue <= Card::Value::WildDraw4; ++eValue) {
			m_vCards.push_back(Card((Card::Color)eColor, (Card::Value)eValue));
		}
	}
}

Hand::Hand(size_t nCards, Hand& ParentHand)
{
	// This constructor creates a child hand from a parent hand.
	// Continue dealing until either nCards is reached or ParentHand is empty.
	while (nCards > 0 && !ParentHand.m_vCards.empty()) {
        --nCards;
		m_vCards.push_back(ParentHand.m_vCards.back());
		ParentHand.m_vCards.pop_back();
	}
}

Hand::~Hand()
{
}

std::string*
Hand::GetHandString()
{
    std::string tops_and_bottoms(" ");

    for (int i = 0; i < m_vCards.size(); ++i)
        tops_and_bottoms += "- ";

    tops_and_bottoms.push_back('\n');

    std::string* hand = new std::string();
    hand->append(tops_and_bottoms);
    for (Card card : m_vCards)
    {
        hand->push_back('|');
        //hand->push_back(static_cast<char>(card.m_eValue) + 48);
        hand->append(card.GetCardString());
    }
    hand->append("|\n");
    hand->append(tops_and_bottoms);
    return hand;
}
