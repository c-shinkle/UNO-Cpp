#include <iostream>
#include "uno/Card.h"

Card::Card(Color eColor, Value eValue) : m_eColor(eColor), m_eValue(eValue)
{
}

Card::~Card()
{
}

std::string
Card::GetCardString(bool bSelected)
{
    //
    // Creates the Card display string. If the Card is selected the colors are reversed.
    //
    // Refer to https://www.lihaoyi.com/post/BuildyourownCommandLinewithANSIescapecodes.html
    const std::string EscapeSeq = "\u001b[";
    const char cResetFont = '0';
    const char cBoldFont = '1';
    const char cFaintFont = '2';
    const char cUnderlineFont = '4';
    const char cSlowBlinkFont = '5';
    const char cForeground = '3';
    const char cBackground = '4';
    const char cBlack = '0';
    const char cRed = '1';
    const char cGreen = '2';
    const char cYellow = '3';
    const char cBlue = '4';
    const char cMagenta = '5';
    const char cCyan = '6';
    const char cWhite = '7';
    const char cDelimeter = ';';
    const char cFinalByte = 'm';

    //
    // Printing Flags
    const bool bBold = true;
    const bool bBackground = bSelected;

    char cCardColor;
    switch (m_eColor) {
    case Color::Red:
        cCardColor = cRed;
        break;
    case Color::Yellow:
        cCardColor = cYellow;
        break;
    case Color::Green:
        cCardColor = cGreen;
        break;
    case Color::Blue :
        cCardColor = cBlue;
        break;
    case Color::Wild:
        cCardColor = cMagenta;
        break;
    default:
        cCardColor = cWhite;
    }

    std::string CardString = EscapeSeq;
    if (bBold) {
        CardString += cBoldFont;
        CardString += cDelimeter;
    }
    if (bBackground) {
        CardString += cBackground;
        CardString += cCardColor;
        CardString += cDelimeter;
        CardString += cForeground;
        CardString += cWhite;
        CardString += cFinalByte;        
    }
    else {
        CardString += cForeground;
        CardString += cCardColor;
        CardString += cFinalByte;        
    }

    char cCardChar = m_eValue + 48;
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

    CardString += cCardChar;
    CardString += EscapeSeq;
    CardString += cResetFont;
    CardString += cFinalByte;

    return CardString;
}