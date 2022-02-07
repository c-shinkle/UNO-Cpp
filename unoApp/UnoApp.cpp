#define OLC_PGE_APPLICATION
#define OLC_IMAGE_STB
#include "olcPixelGameEngine.h"

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		// Name your application
		sAppName = "Example";
	}

	olc::Sprite* sprDemo = nullptr;
	olc::Decal* decDemo = nullptr;
	size_t nRow = 0;
	size_t nCol = 0;
	float timer = 0.0;

public:
	bool OnUserCreate() override
	{
		// Load assets.
		sprDemo = new olc::Sprite("../../../../unoApp/rsrc/uno_deck.png");
		decDemo = new olc::Decal(sprDemo);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::VERY_DARK_BLUE);
		olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY())};

		timer += fElapsedTime;
		//if (false){
		if (timer > 0.5) {
			timer = 0.0;
			nRow = std::rand() % 6;
			size_t nRowLen = nRow == 5 ? 6 : 10;
			nCol = std::rand() % nRowLen;
		}

		olc::vf2d dimCard = { 188, 282 };
		olc::vf2d offsetCard = { 20, 18 };

		offsetCard.x += nCol * dimCard.x;
		offsetCard.y += nRow * dimCard.y;

		DrawPartialDecal(mouse, decDemo, offsetCard, dimCard);

		return true;
	}
};

int main()
{
	Example demo;
	if (demo.Construct(1280, 720, 1, 1))
	//if (demo.Construct(320, 180, 4, 4))
		demo.Start();
	return 0;
}