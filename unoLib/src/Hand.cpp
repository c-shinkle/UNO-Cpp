#include <iostream>
#include <random>
#include "uno/Hand.h"
#include "uno/GameLib.h"

Hand::Hand()
{
}

Hand::~Hand()
{
}

void
Hand::SetFullDeck()
{
    //
    // Clear out any existing cards.
    m_vCards.clear();
    //
    // Create the full deck.
    // For deck composition refer to https://www.letsplayuno.com/news/guide/20181213/30092_732567.html#:~:text=A%20UNO%20deck%20consists%20of,%2C%20yellow%2C%20blue%20and%20green.
    for (Card::Color eColor : Card::ms_vPrimaryColors) {
        for (Card::Value eValue : Card::ms_vValues) {
            // There are 2 of each number (except zero) and action cards per color.
            int nCards = eValue == Card::Value::Zero ? 1 : 2;
            for (int i = 0; i < nCards; ++i)
                m_vCards.push_back(Card((Card::Color)eColor, (Card::Value)eValue));
        }
    }
    // There are 4 of each wild card.
    // Wild, Zero = Regular Wild
    // Wild, Draw = Draw 4 Wild
    for (int i = 0; i < 4; ++i) {
        m_vCards.push_back(Card(Card::Color::Wild, Card::Value::Zero));
        m_vCards.push_back(Card(Card::Color::Wild, Card::Value::Draw));
    }
}

void
Hand::Display(bool bHighlightSelected)
{
    std::string tops_and_bottoms(" ");

    for (int i = 0; i < m_vCards.size(); ++i)
        tops_and_bottoms += "- ";

    tops_and_bottoms.push_back('\n');

    std::string hand;
    hand += tops_and_bottoms;
    for (size_t i = 0; i < m_vCards.size(); ++i) {
        hand += '|';
        if (bHighlightSelected && i == m_nSelected)
            hand += m_vCards[i].GetCardString(true);
        else
            hand += m_vCards[i].GetCardString(false);

    }
    hand += "|\n";
    hand += tops_and_bottoms;
    std::cout << hand;
}

void
Hand::DisplayTopCard()
{
    //
    // Displays the top (back) card in the hand.
    if (m_vCards.empty())
        return;

    std::string tops_and_bottoms(" - \n");
    std::string TopCard = tops_and_bottoms;
    TopCard += '|';
    TopCard += m_vCards.back().GetCardString();
    TopCard += "|\n";
    TopCard += tops_and_bottoms;
    std::cout << TopCard;
}

void
Hand::Shuffle()
{
    //
    // Shuffles the hand according to the Fisher-Yates shuffle O(n).
    if (m_vCards.empty())
        return;
    for (size_t i = m_vCards.size() - 1; i > 0; --i) {
        size_t j = std::rand() % (i + 1);
        std::swap(m_vCards[i], m_vCards[j]);
    }
}

bool
Hand::ReplenishFrom(Hand& DiscardPile)
{
    //
    // Replenishes this hand with the discard pile.
    // Returns true if something goes wrong,
    // otherwise returns false.
    //
    // If the discard pile has less than 2 cards, we can't replenish.
    if (!IsEmpty() || DiscardPile.m_vCards.size() < 2)
        return true;
    std::swap(m_vCards, DiscardPile.m_vCards);
    DiscardPile.m_vCards.push_back(m_vCards.back());
    m_vCards.pop_back();
    //
    // Reset card data and shuffle.
    for (Card& card : m_vCards) {
        card.m_bPlayed = false;
        card.m_eWildColor = Card::Color::Wild;
    }
    Shuffle();
    return false;
}

void
Hand::IncrementSelection(bool bUp)
{
    IncrementIndex(bUp, m_vCards.size(), m_nSelected);
}

bool
Hand::PlayCard(Hand& DiscardPile)
{
    //
    // Returns true if the selected card is successfully played,
    // otherwise returns false.    
    if (!m_vCards[m_nSelected].IsPlayable(DiscardPile.m_vCards.back()))
        return false;
    m_vCards[m_nSelected].m_bPlayed = true;
    //
    // Pick a random wild color.
    if (m_vCards[m_nSelected].m_eColor == Card::Color::Wild)
        m_vCards[m_nSelected].m_eWildColor = (Card::Color)(std::rand() % (4));
    DiscardPile.m_vCards.push_back(m_vCards[m_nSelected]);
    m_vCards.erase(m_vCards.begin() + m_nSelected);
    //
    // Reset the selected card to 0.
    m_nSelected = 0;
    return true;
}

void
Hand::SelectPlayableCard(const Hand& DiscardPile)
{
    //
    // Selects a random playable card from this hand.
    // Not guaranteed to find a playable card.
    std::vector<size_t> vPlayableIndices;
    const Card& TargetCard = DiscardPile.m_vCards.back();
    for (size_t i = 0; i < m_vCards.size(); ++i) {
        if (m_vCards[i].IsPlayable(TargetCard))
            vPlayableIndices.push_back(i);
    }
    if (vPlayableIndices.empty())
        return;
    size_t nRandomIndex = std::rand() % vPlayableIndices.size();
    m_nSelected = vPlayableIndices[nRandomIndex];
}

bool
Hand::IsEmpty()
{
    return m_vCards.empty();
}

size_t
Hand::DealTo(size_t nCards, Hand& TargetHand)
{
    //
    // Returns the number of cards left to be dealt.
    // Continue dealing until either nCards is reached or this hand is empty.
    while (nCards > 0 && !m_vCards.empty()) {
        --nCards;
        TargetHand.m_vCards.push_back(m_vCards.back());
        m_vCards.pop_back();
    }
    return nCards;
}

std::pair<Card::Color, Card::Value>
Hand::GetTopCard()
{
    return { m_vCards.back().m_eColor, m_vCards.back().m_eValue };
}