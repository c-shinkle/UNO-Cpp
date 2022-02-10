#include "uno/Game.h"
#include "uno/GameLib.h"
#include <string>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Game::Game()
{
	// Name your application
	sAppName = "Game";
}

bool Game::OnUserCreate()
{
	//
	// Seed random number generator.
	std::srand((unsigned int)std::time(0));
	// Load assets.
	sprDemo = new olc::Sprite("../../../../unoLib/rsrc/uno_deck.png");
	decDemo = new olc::Decal(sprDemo, true);

	// Set box bounds.
	float TotalWidth = (float)ScreenWidth();
	float TotalHeight = (float)ScreenHeight();
	float midX = TotalWidth / 2.0f;
	float midY = TotalHeight / 2.0f;
	float boxLen = 3.0f * std::min(midX, midY) / 2.0f;
	float boxHeight = dimCard.y;
	// return if bad dimensions
	float xStart = midX - boxLen / 2.0f;
	float yStart = midY - boxLen / 2.0f;
	if ((boxHeight > xStart) || (boxHeight > midY))
		return false;

	m_vRect.push_back({ {xStart, TotalHeight - boxHeight}, {boxLen, boxHeight} }); // Player 0
	m_vRect.push_back({ {0.0, yStart}, {boxHeight, boxLen} }); // Player 1
	m_vRect.push_back({ {xStart, 0.0}, {boxLen, boxHeight} }); // Player 2
	m_vRect.push_back({ {TotalWidth - boxHeight, yStart}, {boxHeight, boxLen} }); // Player 3

	// Create the deck and hands.
	if (InitNewGame())
		return false;

	return true;
}

bool Game::OnUserUpdate(float fElapsedTime)
{
	Clear(olc::VERY_DARK_BLUE);	
	///////////////////////////////////////////////////////////////////////////////////////////////
	// Process Input
	///////////////////////////////////////////////////////////////////////////////////////////////
	Clear(olc::VERY_DARK_BLUE);
	timer += fElapsedTime;

	// Play the current turn when the mouse is clicked.
	if (!m_bGameOver && GetMouse(0).bPressed) {
		SetCurrentHover();
		PlayTurn();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Update Window
	///////////////////////////////////////////////////////////////////////////////////////////////

	// draw lines for the player card boundaries.
	for (const Rect& rect : m_vRect)
		DrawRect(rect.first, rect.second);

	// write the current player index.
	std::string strCurrentIndex = "Index = " + std::to_string(m_nCurrentPlayer);
	DrawString({ 20, 20 }, strCurrentIndex);
	// write the current direction.
	std::string strCurrentDirection = m_bClockwise ? "Direction = CW" : "Direction = CCW";
	DrawString({ 20, 30 }, strCurrentDirection);
	// draw the draw pile
	DrawDrawPile(fElapsedTime);
	// draw the discard pile
	DrawDiscardPile(fElapsedTime);
	// Update hands
	for (size_t i = 0; i < m_vHands.size(); ++i)
		UpdateAndDrawHand(i != 0, fElapsedTime, m_vHands[i]);

	return true;
}

std::pair<int, int>  Game::GetCardImageIndices(bool bBack, const Card& card)
{
	// Returns a pair of indices as { Row, Col } for the given card.
	int nCol = 5;
	int nRow = 4;
	if (bBack)
		return { nCol, nRow };

	switch (card.m_eValue) {
	case Card::Value::Zero: nCol = 9; break;
	case Card::Value::One: nCol = 0; break;
	case Card::Value::Two: nCol = 1; break;
	case Card::Value::Three: nCol = 2; break;
	case Card::Value::Four: nCol = 3; break;
	case Card::Value::Five: nCol = 4; break;
	case Card::Value::Six: nCol = 5; break;
	case Card::Value::Seven: nCol = 6; break;
	case Card::Value::Eight: nCol = 7; break;
	case Card::Value::Nine: nCol = 8; break;
	case Card::Value::Reverse: nRow = 5; break;
	case Card::Value::Skip: nRow = 4; break;
	case Card::Value::Draw: nRow = 4; break;
	}

	bool bAction = card.IsActionCard();
	bool bDraw = card.m_eValue == Card::Value::Draw;

	switch (card.m_eColor) {
	case Card::Color::Red: bAction ? nCol = 0 : nRow = 0; break;
	case Card::Color::Yellow: bAction ? nCol = 1 : nRow = 1; break;
	case Card::Color::Green: bAction ? nCol = 2 : nRow = 2; break;
	case Card::Color::Blue: bAction ? nCol = 3 : nRow = 3; break;
	case Card::Color::Wild:
		if (bDraw)
			return { 4, 8 };
		if (card.m_eValue == Card::Value::Zero)
			return { 4, 9 };
		[[fallthrough]];
	default: return { 5, 4 };
	}

	if (bDraw)
		nCol += 4;
	return { nRow, nCol };
}

olc::vf2d Game::GetCardOffset(bool bBack, const Card& card)
{
	olc::vf2d offsetCard = { 10, 9 };
	auto [nRow, nCol] = GetCardImageIndices(bBack, card);

	offsetCard.x += nCol * dimCard.x;
	offsetCard.y += nRow * dimCard.y;
	return offsetCard;
}

void Game::UpdateCard(float fElapsedTime, Card& card)
{
	// Cache velocity values.
	olc::vf2d oldVelocity = card.m_Velocity;
	// Update position
	card.m_Velocity += fElapsedTime * card.m_Acceleration;
	card.m_Position += fElapsedTime * card.m_Velocity;
	card.m_Angle += fElapsedTime * card.m_AngularVelocity;
	if (card.m_Position.x > ScreenWidth() || card.m_Position.x < 0)
		card.m_Velocity.x *= -1;
	if (card.m_Position.y > ScreenHeight() || card.m_Position.y < 0)
		card.m_Velocity.y *= -1;

	// Stop accelerating if a velocity component becomes zero (crosses zero).
	if (oldVelocity.x * card.m_Velocity.x <= 0) {
		card.m_Velocity.x = 0.0f;
		card.m_Acceleration.x = 0.0f;
	}
	if (oldVelocity.y * card.m_Velocity.y <= 0) {
		card.m_Velocity.y = 0.0f;
		card.m_Acceleration.y = 0.0f;
	}
}

void Game::UpdateBouncingCard(float fElapsedTime, Card& card)
{
	// Update position
	card.m_Position += fElapsedTime * card.m_Velocity;
	card.m_Angle += fElapsedTime * card.m_AngularVelocity;
	if (card.m_Position.x > ScreenWidth() || card.m_Position.x < 0) {
		card.m_Velocity.x *= -1;
		int spinChance = rand() % 2;
		card.m_AngularVelocity /= card.m_AngularVelocity;
		card.m_AngularVelocity *= 33 / (1 + (rand() % 33));
		card.m_AngularVelocity *= spinChance == 1 ? -1 : 1;
	}
	if (card.m_Position.y > ScreenHeight() || card.m_Position.y < 0) {
		card.m_Velocity.y *= -1;

		int spinChance = rand() % 2;
		card.m_AngularVelocity /= card.m_AngularVelocity;
		card.m_AngularVelocity *= -15 / (1 + (rand() % 5));
		card.m_AngularVelocity *= spinChance == 1 ? -1 : 1;
	}
}

void Game::DrawCard(bool bBack, const Card& card)
{
	DrawPartialRotatedDecal(card.m_Position, decDemo, card.m_Angle, dimCard / 2.0, GetCardOffset(bBack, card), dimCard);
}


void Game::PlaceHand(size_t nHand)
{
	// Places the hand for the given player on the window.
	// We will assume for now we always have exaxtly four players.
	if (nHand > 3)
		return;
	
	bool bVert = nHand == 1 || nHand == 3;

	float boxLen = m_vRect[0].second.x;
	float boxHeight = m_vRect[0].second.y;
	float yStart = bVert ? m_vRect[1].first.y : m_vRect[0].first.y + boxHeight / 2.0f;
	float xStart = bVert ? m_vRect[1].first.x + boxHeight / 2.0f : m_vRect[0].first.x;
	if (nHand == 2)
		yStart = m_vRect[2].first.y + boxHeight / 2.0f;
	else if (nHand == 3)
		xStart = m_vRect[3].first.x + boxHeight / 2.0f;


	float Increment = boxLen / (m_vHands[nHand].GetSize() + 1);
	
	for (Card& card : m_vHands[nHand].m_vCards) {
		card.m_bVisible = true;
		card.StopMovement();
		xStart += bVert ? 0.0f : Increment;
		yStart += bVert ? Increment : 0.0f;
		card.m_Position = { xStart, yStart };
		card.m_Angle = (float)(M_PI * nHand / 2.0);
	}
}

void Game::UpdateAndDrawHand(bool bBack, float fElapsedTime, Hand& hand)
{
	for (Card& card : hand.m_vCards) {
		if (card.m_bVisible) {
			UpdateCard(fElapsedTime, card);
			DrawCard(bBack, card);
		}
	}
}

bool Game::IsCardHovered(const Card& card) const
{
	olc::vi2d MousePos = GetMousePos();
	int32_t x1 = (int32_t)card.m_Position.x - (int32_t)dimCard.x / 2;
	int32_t y1 = (int32_t)card.m_Position.y - (int32_t)dimCard.y / 2;
	int32_t x2 = x1 + (int32_t)dimCard.x;
	int32_t y2 = y1 + (int32_t)dimCard.y;

	return MousePos.x >= x1 && MousePos.y >= y1 && MousePos.x <= x2 && MousePos.y <= y2;
}

void Game::SetCurrentHover()
{
	if (IsCardHovered(m_DrawPile.m_vCards.back())) {
		m_eHover = Hoverable::Draw;
		return;
	}

	// See if any cards are hovered over.
	// Take the highest index value as this is the "top" card.
	m_eHover = Hoverable::None;
	for (size_t i = 0; i < m_vHands[0].GetSize(); ++i) {
		if (IsCardHovered(m_vHands[0].m_vCards[i])) {
			m_eHover = Hoverable::Hand;
			m_nHoverIndex = i;
		}
	}
}

void Game::AnimateCardHover(bool bUp, Card& card)
{
	float finalY = m_vRect[0].first.y + m_vRect[0].second.y / 2.0f;
	finalY -= bUp ? dimCard.y / 2.0f : 0.0f;
	olc::vf2d finalPos = {card.m_Position.x, finalY};
	card.AnimateCard(0.5f, finalPos - card.m_Position);
}

void Game::AdvanceCurrentPlayer()
{
	// Place the current hand before advancing.
	PlaceHand(m_nCurrentPlayer);
	IncrementIndex(m_bClockwise, m_nPlayers, m_nCurrentPlayer);
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
		vNames.erase(vNames.begin() + nRandomIndex);
	}
}

bool
Game::InitNewGame()
{
	//
	// The game object will store the current game state,
	// and allows players to play on their turn.
	// Returns true if something goes wrong, otherwise returns false.
	//
	// Create the initial deck (draw pile) and shuffle.
	m_DiscardPile.m_vCards.clear();
	m_DrawPile.SetFullDeck();
	m_DrawPile.Shuffle();
	//
	// Create the hands and assign names.
	CreateHands();
	//
	// Deal out the first cards.
	if (DealCards())
		return true;
	// Place the hands on the screen.
	for (size_t i = 0; i < m_vHands.size(); ++i)
		PlaceHand(i);
	// Set the first player randomly.
	m_nCurrentPlayer = std::rand() % m_nPlayers;
	return false;
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
	PlaceDrawPile();
	PlaceInDiscardPile();
	return false;
}

void
Game::DrawCards(size_t nCards)
{
	//
	// The current player draws nCards and the draw pile is replenished if needed.
	// Sets m_bGameOver to true if something goes wrong.
	nCards = m_DrawPile.DealTo(nCards, m_vHands[m_nCurrentPlayer]);
	if (nCards == 0)
		return;
	//
	// Replenish the draw pile with the discard pile.
	if (m_DrawPile.ReplenishFrom(m_DiscardPile)) {
		std::cout << "ERROR: Ran out of draw pile cards.\n";
		m_bGameOver = true;
		return;
	}
	DrawCards(nCards);
}

void
Game::PlayTurn()
{
	bool bUserTurn = m_nCurrentPlayer == 0;
	if (bUserTurn && m_eHover != Hoverable::Hand) {
		//
		// Draw a card if the user selects the draw option.
		if (m_eHover == Hoverable::Draw) {
			DrawCards(1);
			AdvanceCurrentPlayer();
		}
		return;
	}
	if (bUserTurn)
		m_vHands[0].m_nSelected = m_nHoverIndex;
	else
		m_vHands[m_nCurrentPlayer].SelectPlayableCard(m_DiscardPile);
	//
	// PlayCard checks to see if the selected card is playable.
	if (!m_vHands[m_nCurrentPlayer].PlayCard(m_DiscardPile)) {
		//
		// Wait until the player selects a playable card or the draw option.
		if (bUserTurn)
			return;
		//
		// Draw a card if the selected card is not playable.
		DrawCards(1);
		AdvanceCurrentPlayer();
		return;
	}
	PlaceInDiscardPile();
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
		DrawCards(nDraw);
		AdvanceCurrentPlayer();
	}
	if (eValue == Card::Value::Skip)
		AdvanceCurrentPlayer();
}

void Game::DrawDiscardPile(float fElapsedTime)
{
	// Only draw the last five cards.
	size_t nSize = m_DiscardPile.GetSize();
	size_t i = nSize < 5 ? 0 : nSize - 5;
	for (; i < nSize; ++i) {
		UpdateCard(fElapsedTime, m_DiscardPile.m_vCards[i]);
		DrawCard(false, m_DiscardPile.m_vCards[i]);
	}
}

void Game::DrawDrawPile(float fElapsedTime)
{
	// Only draw the last card.
	if (m_DiscardPile.IsEmpty())
		return;
	UpdateCard(fElapsedTime, m_DrawPile.m_vCards.back());
	DrawCard(true, m_DrawPile.m_vCards.back());
}

void Game::PlaceInDiscardPile()
{
	// Call this method anytime a card is placed in the discard pile.
	if (m_DiscardPile.IsEmpty())
		return;
	Card& card = m_DiscardPile.m_vCards.back();
	card.m_bVisible = true;
	card.StopMovement();
	card.m_Position = { ScreenWidth() / 2.0f, ScreenHeight() / 2.0f };
	// Give a random angle between -60 and 60 degrees.
	float fRandDeg = (float)(rand() % 120);
	card.m_Angle = (float)M_PI * (fRandDeg - 60.0f) / 180.0f;
}

void Game::PlaceDrawPile()
{
	// Positions cards in the Draw pile in the correct position.
	float xPos = ScreenWidth() / 2.0f - dimCard.y;
	float yPos = ScreenHeight() / 2.0f;
	for (Card& card : m_DrawPile.m_vCards) {
		card.StopMovement();
		card.m_Position = { xPos, yPos };
		card.m_Angle = 0.0f;
	}
}