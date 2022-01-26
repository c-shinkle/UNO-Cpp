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

private:
	// Methods
	std::string GetCardString();

	// Members
	Color m_eColor;
	Value m_eValue;
};