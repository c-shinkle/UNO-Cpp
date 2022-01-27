#include <iostream>
#include <string>
#include "uno/GameLib.h"
#include "uno/Hand.h"
#include "uno/Game.h"
//
// Headers for getch() implementation
#ifdef _WIN32
#include "conio.h"
#elif defined __unix__
#include <termios.h>
#else
DEFINE YOUR OS!
#endif


void gameLoop()
{
    Game UNO;
    UNO.Run();    
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

char GetCharInput()
{
    //
    // Used to interact with the console/terminal.
    // Implements getch() on windows/linux.
    // Refer to https://stackoverflow.com/questions/142508/how-do-i-check-os-with-a-preprocessor-directive
    // for preprocessor directives.
#ifdef _WIN32
    return (char)getch();
#elif defined __unix__
    //
    // Refer to https://stackoverflow.com/questions/7469139/what-is-the-equivalent-to-getch-getche-in-linux
    char buf = 0;
    struct termios old = { 0 };
    fflush(stdout);
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
#endif
}

void IncrementIndex(bool bUp, size_t nSize, size_t& nCurrentIndex)
{
    if (nSize == 0)
        return;
    //
    // Selects the next value by either moving up or down.
    if (bUp) {
        ++nCurrentIndex;
        if (nCurrentIndex == nSize)
            nCurrentIndex = 0;
    }
    else {
        if (nCurrentIndex == 0)
            nCurrentIndex = nSize;
        --nCurrentIndex;
    }
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

void KeyInputTest()
{
    //
    // You may need to tweak the GetCharInput() method
    // for your system.
    while (true) {
        char cInput = GetCharInput();
        std::cout << (int)cInput << '\n';
    }
}
