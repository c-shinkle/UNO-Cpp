#include <iostream>
#include "Card.h"

Card::Card(Color eColor, Value eValue) : m_eColor(eColor), m_eValue(eValue)
{
}

Card::~Card()
{
}

void Card::Display()
{
	std::cout << "Color =  " << m_eColor << "\nValue = " << m_eValue << "\n";
}