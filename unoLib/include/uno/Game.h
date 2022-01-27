#pragma once
#include <vector>
#include "uno/Hand.h"

//
// Key defines
#ifdef _WIN32
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ESCAPE 27
#define KEY_ENTER 13
#elif defined __unix__
// Fill in these definitions using GameLib::KeyInputTest()
// (commented out in UnoApp.cpp)
#define KEY_UP ??
#define KEY_DOWN ??
#define KEY_LEFT ??
#define KEY_RIGHT ??
#define KEY_ESCAPE ??
#define KEY_ENTER ??
#endif

class Game {
public:

	// Public Methods
	Game();
	~Game();
	void Run();

private:
	void InitNewGame();
	void AskNumberOfPlayers();
	bool DealCards();
	void DisplayCurrentState();


	size_t m_nInitialHandSize = 7;
	size_t m_nPlayers = 0;
	size_t m_nCurrentPlayer = 0;
	bool m_bClockwise = true;
	Hand m_DiscardPile, m_DrawPile;
	std::vector<Hand> m_vHands;
};