#include <iostream>
#include <vector>
#include "uno/Card.h"
#include "uno/GameLib.h"

//
// Static data
const std::vector<Card::Color> Card::ms_vPrimaryColors = {
    Color::Red, Color::Yellow, Color::Green, Color::Blue };
const std::vector<Card::Value> Card::ms_vValues = {
    Value::Zero, Value::One, Value::Two, Value::Three, Value::Four, Value::Five, Value::Six,
    Value::Seven, Value::Eight, Value::Nine, Value::Reverse, Value::Skip, Value::Draw};

Card::Card(Color eColor, Value eValue) : m_eColor(eColor), m_eValue(eValue)
{
}

Card::~Card()
{
}

bool
Card::IsPlayable(const Card& TargetCard)
{
    //
    // Returns true if this card can be played onto
    // the target card.
    Color eColor = TargetCard.m_eColor == Color::Wild ?
        TargetCard.m_eWildColor : TargetCard.m_eColor;
    if (m_eColor == Color::Wild)
        return true;
    if (m_eColor == eColor)
        return true;
    if (eColor == Color::Wild)
        return true;
    //
    // Prevent (Red, Draw) from playing onto (Wild[Blue], Draw).
    if (m_eValue == TargetCard.m_eValue && TargetCard.m_eColor != Color::Wild)
        return true;
    return false;
}

std::string
Card::GetCardString(bool bSelected)
{
    //
    // Creates the Card display string.
    Color eColor = (m_bPlayed && m_eColor == Color::Wild)
        ? m_eWildColor : m_eColor;
    //
    // Use a more readable color for blue.
    if (eColor == Color::Blue)
        eColor = Color::Cyan;

    char cCardChar = (int)m_eValue + 48;
    if (m_eColor == Color::Wild && m_eValue == Value::Zero)
        cCardChar = '?';
    else if (m_eColor == Color::Wild && m_eValue == Value::Draw)
        cCardChar = '!';
    else if (m_eValue == Value::Draw)
        cCardChar = '+';
    else if (m_eValue == Value::Reverse)
        cCardChar = 'R';
    else if (m_eValue == Value::Skip)
        cCardChar = '%';

    std::string CardString(1, cCardChar);
    ColorString(bSelected, eColor, CardString);

    return CardString;
}