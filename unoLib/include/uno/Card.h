#pragma once

class Card {
public:
	// Enums
	enum Color { Red, Yellow, Green, Blue };
	enum Value { Zero, One, Two, Three, Four, Five, Six, Seven,
		Eight, Nine, Reverse, Skip, Draw2, Wild, WildDraw4 };

	// Public Methods
	Card(Color eColor, Value eValue);
	~Card();
	void Display();

	// Public Members
	Color m_eColor;
	Value m_eValue;
};