/****************************************************************************
 * StockPileView.cpp
 ****************************************************************************/

#include "StockPileView.h"
#include "utils/CardUtils.h"

USING_NS_CC;

namespace
{
const int kPanelW = 150;
const int kPanelH = 200;
}

StockPileView* StockPileView::create()
{
    StockPileView* n = new (std::nothrow) StockPileView();
    if (n && n->init())
    {
        n->autorelease();
        return n;
    }
    CC_SAFE_DELETE(n);
    return nullptr;
}

bool StockPileView::init()
{
    if (!Node::init()) return false;

    setContentSize(Size((float)kPanelW, (float)kPanelH));
    setAnchorPoint(Vec2(0.5f, 0.5f));
    setPosition(Vec2(980.0f, 300.0f));

    layoutVisuals();
    initTouch();
    return true;
}

void StockPileView::layoutVisuals()
{
    if (!_backdrop)
    {
        _backdrop = LayerColor::create(Color4B(26, 72, 48, 210), kPanelW, kPanelH);
        _backdrop->setIgnoreAnchorPointForPosition(false);
        _backdrop->setAnchorPoint(Vec2(0.5f, 0.5f));
        _backdrop->setPosition(getContentSize().width * 0.5f, getContentSize().height * 0.5f);
        addChild(_backdrop, 0);
    }

    if (!_backSprite)
    {
        _backSprite = Sprite::create(CardUtils::getCardBackImagePath());
        if (_backSprite)
        {
            const float tw = _backSprite->getContentSize().width;
            const float th = _backSprite->getContentSize().height;
            const float scale = std::min(120.0f / tw, 160.0f / th);
            _backSprite->setScale(scale);
            _backSprite->setPosition(getContentSize().width * 0.5f, getContentSize().height * 0.55f);
            addChild(_backSprite, 1);
        }
    }

    if (!_countLabel)
    {
        _countLabel = Label::createWithSystemFont("", "Arial", 22);
        _countLabel->setTextColor(Color4B(245, 232, 200, 255));
        _countLabel->setPosition(Vec2(getContentSize().width * 0.5f, 22.0f));
        addChild(_countLabel, 2);
    }
}

void StockPileView::setRemainingCount(int count)
{
    layoutVisuals();
    _drawEnabled = (count > 0);
    setOpacity(_drawEnabled ? 255 : 120);
    if (_countLabel)
    {
        char buf[64];
        snprintf(buf, sizeof(buf), "Stock x%d", count);
        _countLabel->setString(buf);
    }
}

void StockPileView::initTouch()
{
    auto* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(StockPileView::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(StockPileView::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool StockPileView::onTouchBegan(Touch* touch, Event*)
{
    if (!_drawEnabled || !_drawCallback) return false;
    if (!getBoundingBox().containsPoint(touch->getLocation())) return false;
    _touchBegan = touch->getLocation();
    return true;
}

void StockPileView::onTouchEnded(Touch* touch, Event*)
{
    if (!_drawEnabled || !_drawCallback) return;
    if (touch->getLocation().distance(_touchBegan) > 24.0f) return;
    _drawCallback();
}
