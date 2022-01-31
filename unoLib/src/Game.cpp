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
	// Create the hands and assign names.
	CreateHands();
	//
	// Deal out the first cards.
	if (DealCards())
		return;
	//
	// Set the current selectables.
	m_vSelectables.clear();
	AddSelectable(Selectable::Hand);
	if (m_nCurrentPlayer == 0)
		AddSelectable(Selectable::Draw);
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
		std::string strName = m_vHands[i].GetName();
		if (i == m_nCurrentPlayer)
			ColorString(true, Card::Color::White, strName);
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
	// Display the discard pile.
	DisplayDiscardPile();
	//
	// Display the players' cards.
	size_t nDisplay = m_bGameOver ? m_nPlayers : 1;
	for (size_t i = 0; i < nDisplay; ++i) {
		bool bSelectCard = (i == 0) && IsHandSelected();
		m_vHands[i].Display(true, bSelectCard);
	}
}

void
Game::DisplayDiscardPile()
{
	//
	// Displays the top card in the discard pile,
	// and a draw option if it's the player's turn.
	if (m_DiscardPile.IsEmpty())
		return;
	std::cout << "\nDiscard Pile\n";
	std::string tops_and_bottoms(" - \n");
	std::string TopCard = tops_and_bottoms;
	TopCard += '|';
	TopCard += m_DiscardPile.GetCardString(m_DiscardPile.GetSize() - 1);
	TopCard += '|';

	if (m_nCurrentPlayer == 0) {
		TopCard += '\t';
		std::string strDraw = "Draw";
		if (IsDrawSelected())
			ColorString(true, Card::Color::White, strDraw);
		TopCard += strDraw;
	}
	TopCard += '\n';
	TopCard += tops_and_bottoms;
	std::cout << TopCard;
}

void
Game::Run()
{
	InitNewGame();
	while (!m_bGameOver) {
		bool bRedraw = false;
		char cInput = GetCharInput();

		switch (cInput) {
		case KEY_UP:
			bRedraw = true;
			IncrementSelection(true);
			break;
		case KEY_RIGHT:
			if (IsHandSelected()) {
				bRedraw = true;
				m_vHands[0].IncrementSelection(true);
			}
			break;
		case KEY_DOWN:
			bRedraw = true;
			IncrementSelection(false);
			break;
		case KEY_LEFT:
			if (IsHandSelected()) {
				bRedraw = true;
				m_vHands[0].IncrementSelection(false);
			}
			break;
		case KEY_ESCAPE:
			m_bGameOver = true;
			break;
		case KEY_ENTER:
			bRedraw = PlayTurn();
			break;
		}

		if (bRedraw)
			DisplayCurrentState();
	}
	DisplayCurrentState();
}

bool
Game::PlayTurn()
{
	//
	// Returns a bool to indicate if the game needs to be redrawn.
	if (m_nCurrentPlayer != 0)
		m_vHands[m_nCurrentPlayer].SelectPlayableCard(m_DiscardPile);
	//
	// PlayCard checks to see if the selected card is playable.
	if (!m_vHands[m_nCurrentPlayer].PlayCard(m_DiscardPile)) {
		//
		// Draw a card if the selected card is not playable.
		if (DrawCards(1)) {
			std::cout << "ERROR: Ran out of draw pile cards.\n";
			m_bGameOver = true;
			return false;
		}
		AdvanceCurrentPlayer();
		return true;
	}
	//
	// If the current player has no cards, the game is over.
	m_bGameOver = m_vHands[m_nCurrentPlayer].IsEmpty();
	const auto [eColor, eValue] = m_DiscardPile.GetTopCard();
	if (eValue == Card::Value::Reverse)
		m_bClockwise = !m_bClockwise;
	AdvanceCurrentPlayer();
	if (eValue == Card::Value::Draw) {
		size_t nDraw = 2;
		if (eColor == Card::Color::Wild)
			nDraw += 2;
		if (DrawCards(nDraw)) {
			std::cout << "ERROR: Ran out of draw pile cards.\n";
			m_bGameOver = true;
			return false;
		}
		AdvanceCurrentPlayer();
	}
	if (eValue == Card::Value::Skip)
		AdvanceCurrentPlayer();
	return true;
}

void
Game::CreateHands()
{
	//
	// Creates the hands in the deck with a random name.
	std::vector<std::string>vNames = { "You", "Amy","Ann","Joe","Roy","Adam",
		"Alan","Anna","Carl","Emma","Eric","Gary","Jack","Jean","Joan",
		"John","Jose","Juan","Judy","Kyle","Lisa","Mark","Mary","Noah",
		"Paul","Rose","Ruth","Ryan","Sara","Sean","Zach","Aaron","Alice",
		"Amber","Betty","Billy","Bobby","Brian","Bruce","Bryan","Carol",
		"David","Debra","Diana","Diane","Donna","Doris","Dylan","Emily",
		"Ethan","Frank","Grace","Helen","Henry","Jacob","James","Janet",
		"Jason","Jerry","Jesse","Joyce","Julia","Julie","Karen","Kayla",
		"Keith","Kelly","Kevin","Larry","Laura","Linda","Logan","Louis",
		"Maria","Marie","Megan","Nancy","Peter","Ralph","Randy","Roger",
		"Sarah","Scott","Susan","Terry","Tyler","Wayne" };

	m_vHands.clear();
	for (size_t i = 0; i < m_nPlayers; ++i) {
		size_t nRandomIndex = i == 0 ? 0 : std::rand() % vNames.size();
		m_vHands.push_back(Hand(vNames[nRandomIndex]));
	}
}

void
Game::IncrementSelection(bool bUp)
{
	IncrementIndex(bUp, m_vSelectables.size(), m_nSelected);
}

void
Game::AdvanceCurrentPlayer()
{
	if (m_nCurrentPlayer == 0)
		RemoveSelectable(Selectable::Draw);
	IncrementIndex(m_bClockwise, m_nPlayers, m_nCurrentPlayer);
	if (m_nCurrentPlayer == 0)
		AddSelectable(Selectable::Draw);
}

void
Game::AddSelectable(Selectable eSelectable)
{
	//
	// Add the given selectable if it is not
	// already in m_vSelectables.
	for (size_t i = 0; i < m_vSelectables.size(); ++i) {
		if (m_vSelectables[i] == eSelectable)
			return;
	}
	m_vSelectables.push_back(eSelectable);
}

void
Game::RemoveSelectable(Selectable eSelectable)
{
	//
	// Remove the given selectable if it is
	// in m_vSelectables, and update m_nSelected.
	for (size_t i = 0; i < m_vSelectables.size(); ++i) {
		if (m_vSelectables[i] == eSelectable) {
			if (m_nSelected == i)
				IncrementSelection(true);
			m_vSelectables.erase(m_vSelectables.begin() + i);
			return;
		}
	}
}

bool
Game::IsHandSelected()
{
	return m_vSelectables[m_nSelected] == Selectable::Hand;
}

bool
Game::IsDrawSelected()
{
	return m_vSelectables[m_nSelected] == Selectable::Draw;
}
