/****************************************************************************
 * PlayFieldView.cpp - ????????????
 ****************************************************************************/

#include "PlayFieldView.h"

USING_NS_CC;

PlayFieldView* PlayFieldView::create()
{
    PlayFieldView* view = new (std::nothrow) PlayFieldView();
    if (view && view->init())
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool PlayFieldView::init()
{
    if (!Node::init()) return false;
    this->setContentSize(Size(kPlayFieldWidth, kPlayFieldHeight));
    this->setAnchorPoint(Vec2(0, 0));
    this->setPosition(Vec2(0, 580));
    return true;
}

Size PlayFieldView::getPlayFieldSize()
{
    return Size(kPlayFieldWidth, kPlayFieldHeight);
}

void PlayFieldView::setCardClickCallback(const std::function<void(int cardId)>& callback)
{
    _cardClickCallback = callback;
}

void PlayFieldView::addCard(CardView* cardView, const cocos2d::Vec2& position)
{
    if (!cardView) return;
    cardView->setPosition(position);
    this->addChild(cardView);
    if (_cardClickCallback)
    {
        cardView->setClickCallback([this](int cardId) {
            if (_cardClickCallback) _cardClickCallback(cardId);
        });
    }
    _cardViews.push_back(cardView);
}

void PlayFieldView::addCardBack(CardView* cardView, const cocos2d::Vec2& position, int zOrder)
{
    if (!cardView) return;
    cardView->removeFromParent();
    cardView->setPosition(position);
    this->addChild(cardView);
    cardView->setLocalZOrder(zOrder);
    if (_cardClickCallback)
    {
        cardView->setClickCallback([this](int cardId) {
            if (_cardClickCallback) _cardClickCallback(cardId);
        });
    }
    _cardViews.push_back(cardView);
}

void PlayFieldView::removeCard(int cardId)
{
    CardView* cv = takeCard(cardId);
    if (cv) cv->release();
}

CardView* PlayFieldView::takeCard(int cardId)
{
    for (auto it = _cardViews.begin(); it != _cardViews.end(); ++it)
    {
        if ((*it)->getCardId() == cardId)
        {
            CardView* cv = *it;
            cv->retain();
            _cardViews.erase(it);
            cv->removeFromParent();
            return cv;
        }
    }
    return nullptr;
}

CardView* PlayFieldView::getCardById(int cardId)
{
    for (auto* v : _cardViews)
        if (v->getCardId() == cardId) return v;
    return nullptr;
}