#include "olcPixelGameEngine.h"

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:
	Example();
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
	olc::vf2d GetCardOffset(int nRow, int nCol);

public:

	olc::Sprite* sprDemo = nullptr;
	olc::Decal* decDemo = nullptr;
	olc::vf2d dimCard = { 94, 141 };
	int nRow = 0;
	int nCol = 0;
	float timer = 0.0;
};