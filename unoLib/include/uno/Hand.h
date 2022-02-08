#pragma once
#include <vector>
#include <string>
#include "uno/Card.h"

class Hand {
public:

	// Public Methods
	Hand();
	Hand(const std::string& strName);
	~Hand();
	void SetFullDeck();
	void Display(bool bIncludeName = false, bool bHighlightSelected = false);
	void Shuffle();
	bool ReplenishFrom(Hand& DiscardPile);
	void IncrementSelection(bool bUp);
	bool PlayCard(Hand& DiscardPile);
	void SelectPlayableCard(const Hand& DiscardPile);
	bool IsEmpty();
	size_t GetSize();
	std::string GetName();
	std::string GetCardString(size_t nIndex);
	size_t DealTo(size_t nCards, Hand& TargetHand);
	std::pair<Card::Color, Card::Value> GetTopCard();

	std::vector<Card> m_vCards;
private:
	// Private Members
	size_t m_nSelected = 0;
	std::string m_strName;
};