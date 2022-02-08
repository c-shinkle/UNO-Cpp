#include "uno/Example.h"

Example::Example()
{
	// Name your application
	sAppName = "Example";
}

bool Example::OnUserCreate()
{
	//
	// Seed random number generator.
	std::srand((unsigned int)std::time(0));
	// Load assets.
	sprDemo = new olc::Sprite("../../../../unoLib/rsrc/uno_deck.png");
	decDemo = new olc::Decal(sprDemo);

	// Create bouncing cards.
	fullDeck.SetFullDeck();
	fullDeck.Shuffle();
	for (Card& card : fullDeck.m_vCards) {
		card.m_Velocity = { (float)(500 + (rand() % 250)), (float)(250 + (rand() % 250)) };
		card.m_AngularVelocity = 8 + 6 / (1 + (rand() % 15));
	}

	// Set box bounds.
	float TotalWidth = ScreenWidth();
	float TotalHeight = ScreenHeight();
	float midX = TotalWidth / 2.0;
	float midY = TotalHeight / 2.0;
	float boxLen = 3.0 * std::min(midX, midY) / 2.0;
	float boxHeight = dimCard.y;
	// return if bad dimensions
	float xStart = midX - boxLen / 2.0;
	float yStart = midY - boxLen / 2.0;
	if ((boxHeight > xStart) || (boxHeight > midY))
		return false;

	m_vRect.push_back({ {xStart, TotalHeight - boxHeight}, {boxLen, boxHeight} }); // Player 0
	m_vRect.push_back({ {0.0, yStart}, {boxHeight, boxLen} }); // Player 1
	m_vRect.push_back({ {xStart, 0.0}, {boxLen, boxHeight} }); // Player 2
	m_vRect.push_back({ {TotalWidth - boxHeight, yStart}, {boxHeight, boxLen} }); // Player 3

	// create user hand
	m_vHands.resize(1);

	return true;
}

bool Example::OnUserUpdate(float fElapsedTime)
{
	Clear(olc::VERY_DARK_BLUE);
	olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY()) };

	timer += fElapsedTime;

	// draw lines for the player card boundaries.
	for (const Rect& rect : m_vRect)
		DrawRect(rect.first, rect.second);

	// Update hands
	UpdateAndDrawHand(fElapsedTime, fullDeck);
	for (Hand& hand : m_vHands)
		UpdateAndDrawHand(fElapsedTime, hand);


	// update player hand second
	if (timer < 1)
		return true;
	timer = 0.0;

	static bool bIncrease = true;
	if (m_vHands[0].GetSize() == 0) {
		bIncrease = true;
		fullDeck.Shuffle();
	}
	else if (m_vHands[0].GetSize() >= 15)
		bIncrease = false;

	if (bIncrease)
		fullDeck.DealTo(1, m_vHands[0]);
	else {
		m_vHands[0].m_vCards.back().m_bVisible = false;
		m_vHands[0].DealTo(1, fullDeck);
	}

	PlaceHand(0);

	return true;
}

std::pair<int, int>  Example::GetCardImageIndices(bool bBack, const Card& card)
{
	// Returns a pair of indices as { Row, Col } for the given card.
	// This code is ugly, but it will be fixed after rearranging the card image file.
	if (bBack)
		return { 5, 6 };


	switch (card.m_eColor) {
	case Card::Color::Red:
		switch (card.m_eValue) {
		case Card::Value::Zero: return { 5, 4 };
		case Card::Value::One: return { 0, 0 };
		case Card::Value::Two: return { 0, 1 };
		case Card::Value::Three: return { 0, 2 };
		case Card::Value::Four: return { 0, 3 };
		case Card::Value::Five: return { 0, 4 };
		case Card::Value::Six: return { 0, 5 };
		case Card::Value::Seven: return { 0, 6 };
		case Card::Value::Eight: return { 0, 7 };
		case Card::Value::Nine: return { 0, 8 };
		case Card::Value::Reverse: return { 4, 8 };
		case Card::Value::Skip: return { 4, 0 };
		case Card::Value::Draw: return { 4, 4 };
		}
	case Card::Color::Yellow:
		switch (card.m_eValue) {
		case Card::Value::Zero: return { 5, 5 };
		case Card::Value::One: return { 1, 0 };
		case Card::Value::Two: return { 1, 1 };
		case Card::Value::Three: return { 1, 2 };
		case Card::Value::Four: return { 1, 3 };
		case Card::Value::Five: return { 1, 4 };
		case Card::Value::Six: return { 1, 5 };
		case Card::Value::Seven: return { 1, 6 };
		case Card::Value::Eight: return { 1, 7 };
		case Card::Value::Nine: return { 1, 8 };
		case Card::Value::Reverse: return { 4, 9 };
		case Card::Value::Skip: return { 4, 1 };
		case Card::Value::Draw: return { 4, 5 };
		}
	case Card::Color::Green:
		switch (card.m_eValue) {
		case Card::Value::Zero: return { 5, 3 };
		case Card::Value::One: return { 3, 0 };
		case Card::Value::Two: return { 3, 1 };
		case Card::Value::Three: return { 3, 2 };
		case Card::Value::Four: return { 3, 3 };
		case Card::Value::Five: return { 3, 4 };
		case Card::Value::Six: return { 3, 5 };
		case Card::Value::Seven: return { 3, 6 };
		case Card::Value::Eight: return { 3, 7 };
		case Card::Value::Nine: return { 3, 8 };
		case Card::Value::Reverse: return { 5, 1 };
		case Card::Value::Skip: return { 4, 3 };
		case Card::Value::Draw: return { 4, 7 };
		}
	case Card::Color::Blue:
		switch (card.m_eValue) {
		case Card::Value::Zero: return { 5, 2 };
		case Card::Value::One: return { 2, 0 };
		case Card::Value::Two: return { 2, 1 };
		case Card::Value::Three: return { 2, 2 };
		case Card::Value::Four: return { 2, 3 };
		case Card::Value::Five: return { 2, 4 };
		case Card::Value::Six: return { 2, 5 };
		case Card::Value::Seven: return { 2, 6 };
		case Card::Value::Eight: return { 2, 7 };
		case Card::Value::Nine: return { 2, 8 };
		case Card::Value::Reverse: return { 5, 0 };
		case Card::Value::Skip: return { 4, 2 };
		case Card::Value::Draw: return { 4, 6 };
		}
	case Card::Color::Wild:
		switch (card.m_eValue) {
		case Card::Value::Zero: return { 0, 9 };
		case Card::Value::Draw: return { 3, 9 };
		}
	}
	// By default return the back side.
	return { 5, 6 };

}

olc::vf2d Example::GetCardOffset(bool bBack, const Card& card)
{
	olc::vf2d offsetCard = { 10, 9 };
	auto [nRow, nCol] = GetCardImageIndices(bBack, card);

	offsetCard.x += nCol * dimCard.x;
	offsetCard.y += nRow * dimCard.y;
	return offsetCard;
}

void Example::UpdateCard(float fElapsedTime, Card& card)
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

void Example::DrawCard(const Card& card)
{
	DrawPartialRotatedDecal(card.m_Position, decDemo, card.m_Angle, dimCard / 2.0, GetCardOffset(false, card), dimCard);
}


void Example::PlaceHand(size_t nHand)
{
	// Places the hand for the given player on the window.
	// We will assume for now we always have exaxtly four players.
	
	// start with user's hand...
	if (nHand != 0)
		return;
	
	float boxLen = m_vRect[0].second.x;
	float boxHeight = m_vRect[0].second.y;
	float yPos = m_vRect[0].first.y + boxHeight / 2.0;
	float xStart = m_vRect[0].first.x;
	float xIncrement = boxLen / (m_vHands[nHand].GetSize() + 1);
	
	for (Card& card : m_vHands[nHand].m_vCards) {
		card.m_bVisible = true;
		card.StopMovement();
		xStart += xIncrement;
		card.m_Position.x = xStart;
		card.m_Position.y = yPos;
	}
}


void Example::UpdateAndDrawHand(float fElapsedTime, Hand& hand)
{
	for (Card& card : hand.m_vCards) {
		if (card.m_bVisible) {
			UpdateCard(fElapsedTime, card);
			DrawCard(card);
		}
	}
}