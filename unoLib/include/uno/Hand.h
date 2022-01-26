#pragma once
#include <vector>
#include <string>
#include "uno/Card.h"

class Hand {
public:

	// Public Methods
	Hand();
	Hand(size_t nCards, Hand& ParentHand);
	~Hand();
	std::string* GetHandString();
	void Shuffle();

private:
	std::vector<Card> m_vCards;
};