#include <iostream>
#include <random>
#include "uno/Hand.h"

Hand::Hand()
{
    //
	// Default constructor creates the full deck.
    // For deck composition refer to https://www.letsplayuno.com/news/guide/20181213/30092_732567.html#:~:text=A%20UNO%20deck%20consists%20of,%2C%20yellow%2C%20blue%20and%20green.
	for (int eColor = Card::Color::Red; eColor <= Card::Color::Blue; ++eColor) {
        // There is only 1 zero per deck.
        m_vCards.push_back(Card((Card::Color)eColor, Card::Value::Zero));
        // There are 2 of each other number and action cards.
		for (int eValue = Card::Value::One; eValue <= Card::Value::Draw; ++eValue) {
            for (int i = 0; i < 2; ++i)
			    m_vCards.push_back(Card((Card::Color)eColor, (Card::Value)eValue));
		}
	}
    // There are 4 of each wild card.
    // Wild, Zero = Regular Wild
    // Wild, Draw = Draw 4 Wild
    for (int i = 0; i < 4; ++i) {
        m_vCards.push_back(Card(Card::Color::Wild, Card::Value::Zero));
        m_vCards.push_back(Card(Card::Color::Wild, Card::Value::Draw));
    }
}

Hand::Hand(size_t nCards, Hand& ParentHand)
{
    //
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

void
Hand::Shuffle()
{
    //
    // Shuffles the hand according to the Fisher-Yates shuffle O(n).
    size_t n = m_vCards.size();
    for (size_t i = n - 1; i > 0; --i) {
        size_t j = std::rand() % (i + 1);
        std::swap(m_vCards[i], m_vCards[j]);
    }
}
