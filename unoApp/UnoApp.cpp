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
		DrawDecal(mouse, decDemo);

		return true;
	}
};

int main()
{
	Example demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
	return 0;
}