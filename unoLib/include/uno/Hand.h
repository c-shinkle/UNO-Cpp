#pragma once
#include <vector>
#include <string>
#include "uno/Card.h"

class Hand {
public:

	// Public Methods
	Hand();
	~Hand();
	void SetFullDeck();
	void Display();
	void DisplayTopCard();
	void Shuffle();
	size_t DealTo(size_t nCards, Hand& TargetHand);

private:
	std::vector<Card> m_vCards;
};