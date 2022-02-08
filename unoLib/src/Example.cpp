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
		card.m_AngularVelocity = 8.0f + 6.0f / (1.0f + (rand() % 15));
	}

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

	// create user hand
	m_vHands.resize(1);

	// place user hand
	fullDeck.DealTo(7, m_vHands[0]);
	PlaceHand(0);

	m_vHands[0].m_vCards.back().AnimateCard(0.75f, {0, -80});

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



	return true;
}

std::pair<int, int>  Example::GetCardImageIndices(bool bBack, const Card& card)
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

void Example::UpdateBouncingCard(float fElapsedTime, Card& card)
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
	float yPos = m_vRect[0].first.y + boxHeight / 2.0f;
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