#include "olcPixelGameEngine.h"
#include "uno/Card.h"
#include "uno/Hand.h"

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:
	Example();
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
	std::pair<int, int> GetCardImageIndices(bool bBack, const Card& card);
	olc::vf2d GetCardOffset(bool bBack, const Card& card);
	void UpdateCard(float fElapsedTime, Card& card);
	void DrawCard(const Card& card);
	void PlaceHand(size_t nHand);
	void UpdateAndDrawHand(float fElapsedTime, Hand& hand);

public:

	typedef std::pair<olc::vf2d, olc::vf2d> Rect;
	std::vector<Rect> m_vRect;

	olc::Sprite* sprDemo = nullptr;
	olc::Decal* decDemo = nullptr;
	olc::vf2d dimCard = { 94, 141 };
	Hand fullDeck;
	int nRow = 0;
	int nCol = 0;
	float timer = 0.0;
	std::vector<Hand> m_vHands;
};