/****************************************************************************
 * HandPileView.cpp - ????????????
 ****************************************************************************/

#include "HandPileView.h"
#include "models/CardModel.h"

USING_NS_CC;

HandPileView* HandPileView::create()
{
    HandPileView* view = new (std::nothrow) HandPileView();
    if (view && view->init())
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool HandPileView::init()
{
    if (!Node::init()) return false;
    this->setContentSize(Size(kHandPileWidth, kHandPileHeight));
    this->setAnchorPoint(Vec2(0, 0));
    this->setPosition(Vec2(0, 0));
    return true;
}

Size HandPileView::getHandPileSize()
{
    return Size(kHandPileWidth, kHandPileHeight);
}

Vec2 HandPileView::getTopCardPosition()
{
    return Vec2(kHandPileWidth / 2.0f, kHandPileHeight / 2.0f + 100);
}

void HandPileView::setCardClickCallback(const std::function<void(int cardId)>& callback)
{
    _cardClickCallback = callback;
}

void HandPileView::addCardToTop(CardView* cardView)
{
    if (!cardView) return;
    Vec2 topPos = getTopCardPosition();
    cardView->removeFromParent();
    cardView->setPosition(topPos);
    this->addChild(cardView);
    if (_cardClickCallback)
    {
        cardView->setClickCallback([this](int cardId) {
            if (_cardClickCallback) _cardClickCallback(cardId);
        });
    }
    _cardViews.push_back(cardView);
    updateCardPositions();
}

void HandPileView::removeCard(int cardId)
{
    CardView* cv = takeCard(cardId);
    if (cv) cv->release();
}

CardView* HandPileView::takeCard(int cardId)
{
    for (auto it = _cardViews.begin(); it != _cardViews.end(); ++it)
    {
        if ((*it)->getCardId() == cardId)
        {
            CardView* cv = *it;
            cv->retain();
            _cardViews.erase(it);
            cv->removeFromParent();
            updateCardPositions();
            return cv;
        }
    }
    return nullptr;
}

CardView* HandPileView::getCardById(int cardId)
{
    for (auto* v : _cardViews)
        if (v->getCardId() == cardId) return v;
    return nullptr;
}

CardView* HandPileView::getTopCard()
{
    if (_cardViews.empty()) return nullptr;
    return _cardViews.back();
}

void HandPileView::reorderCardsToMatch(const std::vector<CardModel*>& handModels)
{
    if (handModels.size() != _cardViews.size()) return;

    std::vector<CardView*> next;
    next.reserve(handModels.size());
    for (CardModel* m : handModels)
    {
        if (!m) return;
        CardView* v = getCardById(m->cardId);
        if (!v) return;
        next.push_back(v);
    }
    _cardViews.swap(next);
    updateCardPositions();
}

void HandPileView::updateCardPositions()
{
    Vec2 topPos = getTopCardPosition();
    for (size_t i = 0; i < _cardViews.size(); i++)
    {
        int offset = (int)(_cardViews.size() - 1 - i);
        Vec2 pos = Vec2(topPos.x, topPos.y - offset * kCardStackOffsetY);
        _cardViews[i]->setPosition(pos);
        _cardViews[i]->setLocalZOrder((int)i);
    }
}