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
	bool ReplenishFrom(Hand& DiscardPile);
	void IncrementSelection(bool bUp);
	bool PlayCard(Hand& DiscardPile);
	void SelectPlayableCard(const Hand& DiscardPile);
	bool IsEmpty();
	size_t GetSize();
	size_t DealTo(size_t nCards, Hand& TargetHand);
	std::pair<Card::Color, Card::Value> GetTopCard();

	// Public Members
	std::string m_strName;
private:
	// Private Members
	size_t m_nSelected = 0;
	std::vector<Card> m_vCards;
};