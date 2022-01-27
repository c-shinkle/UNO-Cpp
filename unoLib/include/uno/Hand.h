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
	void Display(bool bHighlightSelected = false);
	void DisplayTopCard();
	void Shuffle();
	size_t DealTo(size_t nCards, Hand& TargetHand);

private:
	size_t m_nSelected = 0;
	std::vector<Card> m_vCards;
};