#include "olcPixelGameEngine.h"
#include "uno/Card.h"
#include "uno/Hand.h"

// Override base class with your custom functionality
class Game : public olc::PixelGameEngine
{
public:
	// Enums
	enum class Hoverable { None, Hand, Draw };

	Game();
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
	std::pair<int, int> GetCardImageIndices(bool bBack, const Card& card);
	olc::vf2d GetCardOffset(bool bBack, const Card& card);
	void UpdateCard(float fElapsedTime, Card& card);
	void UpdateBouncingCard(float fElapsedTime, Card& card);
	void DrawCard(bool bBack, const Card& card);
	void PlaceHand(size_t nHand);
	void UpdateAndDrawHand(bool bBack, float fElapsedTime, Hand& hand);
	bool IsCardHovered(const Card& card) const;
	void SetCurrentHover();
	void AnimateCardHover(bool bUp, Card& card);
	void AdvanceCurrentPlayer();
	void CreateHands();
	bool InitNewGame();
	bool DealCards();
	void DrawCards(size_t nCards);
	void PlayTurn();
	void DrawDiscardPile(float fElapsedTime);
	void DrawDrawPile(float fElapsedTime);
	void PlaceInDiscardPile();
	void PlaceDrawPile();

public:

	typedef std::pair<olc::vf2d, olc::vf2d> Rect;
	std::vector<Rect> m_vRect;

	olc::Sprite* sprDemo = nullptr;
	olc::Decal* decDemo = nullptr;
	olc::vf2d dimCard = { 94, 141 };

	Hoverable m_eHover = Hoverable::None;
	size_t m_nHoverIndex = 0;
	float timer = 0.0;


	bool m_bClockwise = true;
	bool m_bGameOver = false;
	const size_t m_nInitialHandSize = 7;
	const size_t m_nPlayers = 4;
	size_t m_nCurrentPlayer = 0;
	Hand m_DiscardPile, m_DrawPile;
	std::vector<Hand> m_vHands;
};