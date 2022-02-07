#include "uno/Example.h"

Example::Example()
{
	// Name your application
	sAppName = "Example";
}

bool Example::OnUserCreate()
{
	// Load assets.
	sprDemo = new olc::Sprite("../../../../unoLib/rsrc/uno_deck.png");
	decDemo = new olc::Decal(sprDemo);
	return true;
}

bool Example::OnUserUpdate(float fElapsedTime)
{
	Clear(olc::VERY_DARK_BLUE);
	olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY()) };

	timer += fElapsedTime;
	//if (false){
	if (timer > 0.5) {
		timer = 0.0;
		nRow = std::rand() % 6;
		size_t nRowLen = nRow == 5 ? 7 : 10;
		nCol = std::rand() % nRowLen;
	}

	olc::vf2d dimCard = { 94, 141 };
	olc::vf2d offsetCard = { 10, 9 };

	offsetCard.x += nCol * dimCard.x;
	offsetCard.y += nRow * dimCard.y;

	DrawPartialDecal(mouse, decDemo, offsetCard, dimCard);

	return true;
}