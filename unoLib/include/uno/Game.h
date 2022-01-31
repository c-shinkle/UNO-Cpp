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
#elif defined __APPLE__
// Fill in these definitions using GameLib::KeyInputTest()
// (commented out in UnoApp.cpp)
#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_LEFT 68
#define KEY_RIGHT 67
#define KEY_ESCAPE 27
#define KEY_ENTER 10
#endif

class Game {
public:
	// Enums
	enum class Selectable { Hand, Draw };

	// Public Methods
	Game();
	~Game();
	void Run();

private:
	// Private Methods
	void InitNewGame();
	void AskNumberOfPlayers();
	bool DealCards();
	bool DrawCards(size_t nCards);
	void DisplayCurrentState();
	void DisplayDiscardPile();
	void CreateHands();
	bool PlayTurn();
	void IncrementSelection(bool bUp);
	void AdvanceCurrentPlayer();
	void AddSelectable(Selectable eSelectable);
	void RemoveSelectable(Selectable eSelectable);
	bool IsHandSelected();
	bool IsDrawSelected();

	// Private Members
	bool m_bClockwise = true;
	bool m_bGameOver = false;
	size_t m_nInitialHandSize = 7;
	size_t m_nPlayers = 0;
	size_t m_nCurrentPlayer = 0;
	size_t m_nSelected = 0;
	Hand m_DiscardPile, m_DrawPile;
	std::vector<Selectable> m_vSelectables;
	std::vector<Hand> m_vHands;
};