#pragma once
#include <vector>
#include "uno/Hand.h"

class Game {
public:

	// Public Methods
	Game();
	~Game();
	void AskNumberOfPlayers();
	bool DealCards();
	void InitNewGame();
	void DisplayCurrentState();

private:
	size_t m_nInitialHandSize = 7;
	size_t m_nPlayers = 0;
	size_t m_nCurrentPlayer = 0;
	bool m_bClockwise = true;
	Hand m_DiscardPile, m_DrawPile;
	std::vector<Hand> m_vHands;
};