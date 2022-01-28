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

bool
Game::DrawCards(size_t nCards)
{
	//
	// The current player draws nCards and the draw pile is replenished if needed.
	// Returns true if something goes wrong, otherwise returns false.
	nCards = m_DrawPile.DealTo(nCards, m_vHands[m_nCurrentPlayer]);
	if (nCards == 0)
		return false;
	//
	// Replenish the draw pile with the discard pile.
	if (m_DrawPile.ReplenishFrom(m_DiscardPile))
		return true;
	return DrawCards(nCards);
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
	// Create a direction string.
	char cDirection = m_bClockwise ? '>' : '<';
	std::string strDirection(100, cDirection);
	//
	// Create Player and Cards strings.
	std::string strPlayers = "Player:";
	std::string strCards = "Cards:";
	for (size_t i = 0; i < m_vHands.size(); ++i) {
		strPlayers += '\t';
		std::string strName = std::to_string(i);
		if (i == 0)
			strName += "(You)";
		if (i == m_nCurrentPlayer)
			ColorString(true, Card::Color::Blue, strName);
		strPlayers += strName;
		strCards += '\t';
		strCards += std::to_string(m_vHands[i].GetSize());
	}
	//
	// Display direction, players, and cards.
	// Clockwise corresponds to increasing player index.
	std::cout << strDirection << '\n';
	std::cout << strPlayers << '\n';
	std::cout << strCards << '\n';
	std::cout << strDirection << '\n';
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
Game::DisplayEndGameState()
{
	//
	// Displays the end game state.
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
	// Display the players' cards.
	for (size_t i = 0; i < m_nPlayers; ++i) {
		std::cout << "\nPlayer " << i;
		if (i == 0)
			std::cout << " (You)";
		std::cout << "\n";
		m_vHands[i].Display(true);
	}
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
			m_vHands[0].IncrementSelection(true);
			break;
		case KEY_DOWN:
		case KEY_LEFT:
			bRedraw = true;
			m_vHands[0].IncrementSelection(false);
			break;
		case KEY_ESCAPE:
			bGameOver = true;
			break;
		case KEY_ENTER:
			std::tie(bRedraw, bGameOver) = PlayTurn();
			break;
		}

		if (bRedraw)
			DisplayCurrentState();
	}
	DisplayEndGameState();
}

std::pair<bool, bool>
Game::PlayTurn()
{
	//
	// Returns a pair of bools to indicate if the game needs to be redrawn
	// or the game has ended. <bRedraw, bGameOver>
	if (m_nCurrentPlayer != 0)
		m_vHands[m_nCurrentPlayer].SelectPlayableCard(m_DiscardPile);
	//
	// PlayCard checks to see if the selected card is playable.
	if (!m_vHands[m_nCurrentPlayer].PlayCard(m_DiscardPile)) {
		//
		// Draw a card if the selected card is not playable.
		if (DrawCards(1)) {
			std::cout << "ERROR: Ran out of draw pile cards.\n";
			return { false, true };
		}
		//
		// Advance the current player.
		IncrementIndex(m_bClockwise, m_nPlayers, m_nCurrentPlayer);
		return { true, false };
	}
	//
	// If the current player has no cards, the game is over.
	bool bGameOver = m_vHands[m_nCurrentPlayer].IsEmpty();
	const auto [eColor, eValue] = m_DiscardPile.GetTopCard();
	if (eValue == Card::Value::Reverse)
		m_bClockwise = !m_bClockwise;
	//
	// Advance the current player.
	IncrementIndex(m_bClockwise, m_nPlayers, m_nCurrentPlayer);
	if (eValue == Card::Value::Draw) {
		size_t nDraw = 2;
		if (eColor == Card::Color::Wild)
			nDraw += 2;
		if (DrawCards(nDraw)) {
			std::cout << "ERROR: Ran out of draw pile cards.\n";
			return { false, true };
		}
		IncrementIndex(m_bClockwise, m_nPlayers, m_nCurrentPlayer);
	}
	if (eValue == Card::Value::Skip)
		IncrementIndex(m_bClockwise, m_nPlayers, m_nCurrentPlayer);
	return { true, bGameOver };
}
