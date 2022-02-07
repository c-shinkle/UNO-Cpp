#include "olcPixelGameEngine.h"

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:
	Example();
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;

public:

	olc::Sprite* sprDemo = nullptr;
	olc::Decal* decDemo = nullptr;
	size_t nRow = 0;
	size_t nCol = 0;
	float timer = 0.0;
};