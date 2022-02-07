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
	if (timer > 1) {
		timer = 0.0;
		nRow = std::rand() % 6;
		int nRowLen = nRow == 5 ? 7 : 10;
		nCol = std::rand() % nRowLen;
	}

	olc::vf2d screenCenter = dimCard / 2.0;

	DrawPartialRotatedDecal(mouse, decDemo, timer * 3.14159, screenCenter, GetCardOffset(nRow, nCol), dimCard);
	//DrawPartialDecal(mouse, decDemo, GetCardOffset(nRow, nCol), dimCard);

	return true;
}

olc::vf2d Example::GetCardOffset(int nRow, int nCol)
{
	olc::vf2d offsetCard = { 10, 9 };

	offsetCard.x += nCol * dimCard.x;
	offsetCard.y += nRow * dimCard.y;
	return offsetCard;
}