#pragma once
#include <string>

class Card {
	friend class Hand;
public:
	// Enums
	enum Color { Red, Yellow, Green, Blue, Wild };
	enum Value {
		Zero, One, Two, Three, Four, Five, Six, Seven,
		Eight, Nine, Reverse, Skip, Draw
	};
	Card(Color eColor, Value eValue);
	~Card();
	bool IsPlayable(const Card& TargetCard);

private:
	// Methods
	std::string GetCardString(bool bSelected = false);

	// Members
	Color m_eColor;
	Value m_eValue;
};