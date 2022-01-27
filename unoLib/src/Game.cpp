#include <iostream>
#include "uno/Game.h"
#include "uno/GameLib.h"

Game::Game()
{
}

Game::~Game()
{
}

void
Game::AskNumberOfPlayers()
{
	while (m_nPlayers < 2 || m_nPlayers > 10) {
		std::cout << "Enter number of players: ";
		std::cin >> m_nPlayers;
		if (std::cin.fail()) {
			std::cout << "ERROR: You did not enter an integer.\n";
			// Get rid of the failure state.
			std::cin.clear();
			// Discard 'bad' character(s) 
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else if (m_nPlayers < 2 || m_nPlayers > 10)
			std::cout << "ERROR: Number of players must be between 2-10.\n";
	}
	std::cout << m_nPlayers << " players selected!\n";
	//
	// Set the first player randomly.
	m_nCurrentPlayer = std::rand() % m_nPlayers;
}

bool
Game::DealCards()
{
	//
	// Deals the InitialHandSize to all players, 1 card at a time.
	// Returns true if something goes wrong, otherwise returns false.
	for (size_t i = 0; i < m_nInitialHandSize; ++i) {
		//
		// Inner loop is the players (matches real dealing)
		for (Hand& PlayerHand : m_vHands) {
			if (m_DrawPile.DealTo(1, PlayerHand) != 0) {
				std::cout << "ERROR: Ran out of cards in the draw pile.\n";
				return true;
			}
		}
	}
	//
	// Deal a card to the discard pile.
	if (m_DrawPile.DealTo(1, m_DiscardPile) != 0) {
		std::cout << "ERROR: Ran out of cards in the draw pile.\n";
		return true;
	}
	return false;
}

void
Game::InitNewGame()
{
	//
	// The game object will store the current game state,
	// and allows players to play on their turn.
	// Returns true if something goes wrong, otherwise returns false.
	AskNumberOfPlayers();
	//
	// Create the initial deck (draw pile) and shuffle.
	m_DrawPile.SetFullDeck();
	m_DrawPile.Shuffle();
	//
	// Create the hands.
	m_vHands.resize(m_nPlayers);
	//
	// Deal out the first cards.
	if (DealCards())
		return;	
	//
	// Display the game.
	DisplayCurrentState();
}

void
Game::DisplayCurrentState()
{
	//
	// Displays the current game state.
	//
	// First, clear the screen for a fresh display.
	ClearScreen();
	//
	// Display current player, and direction.
	// Clockwise corresponds to increasing player index.
	std::cout << "Player\t\tDirection\n" << m_nCurrentPlayer;
	if (m_nCurrentPlayer == 0)
		std::cout << "(You)";
	if (m_bClockwise)
		std::cout << "\t\tCW(>)\n";
	else
		std::cout << "\t\tCCW(<)\n";
	//
	// Display the top (back) of the discard pile.
	std::cout << "\nDiscard Pile\n";
	m_DiscardPile.DisplayTopCard();
	//
	// Display the player's cards.
	std::cout << "\nYour Cards\n";
	m_vHands[0].Display(true);
}

void
Game::Run()
{
	InitNewGame();
	bool bGameOver = false;
	while (!bGameOver) {
		bool bRedraw = false;
		char cInput = GetCharInput();

		switch (cInput) {
		case KEY_UP:
		case KEY_RIGHT:
			bRedraw = true;
			m_vHands[0].Select(true);
			break;
		case KEY_DOWN:
		case KEY_LEFT:
			bRedraw = true;
			m_vHands[0].Select(false);
			break;
		case KEY_ESCAPE:
			bGameOver = true;
			break;
		}

		if (bRedraw)
			DisplayCurrentState();
	}
}
