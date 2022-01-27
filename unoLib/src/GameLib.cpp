#include <iostream>
#include <string>
#include "uno/GameLib.h"
#include "uno/Hand.h"
#include "uno/Game.h"

void gameLoop()
{
    Game UNO;
    UNO.InitNewGame();
}

void ClearScreen()
{
    std::cout << "\033[2J\033[H";
}

void PrintBorder()
{
    size_t nBorderLen = 100;
    std::string strStarRow(nBorderLen, '*');
    std::string strSlashRow(nBorderLen, '/');
    std::cout << strStarRow << '\n' << strSlashRow << '\n' << strStarRow << '\n';
}

void FeatureTest()
{
    PrintBorder();
    std::cout << "\nFEATURE TEST\n\n";

    Hand Deck;
    Deck.SetFullDeck();
    //
    // Before shuffling.
    Deck.Display();
    //
    // After shuffling.
    Deck.Shuffle();
    Deck.Display();

    std::cout << "\n\n";
    PrintBorder();
    std::cout << "Press enter to continue...";
    std::getline(std::cin, std::string());

    ClearScreen();
}
