/****************************************************************************
 * CardView.cpp - ???????????? + ?????????? + ??????? + ?????
 ****************************************************************************/

#include "CardView.h"
#include "utils/CardUtils.h"

USING_NS_CC;

namespace
{
bool fileExists(const std::string& path)
{
    return FileUtils::getInstance()->isFileExist(path);
}
}

void CardView::scaleSpriteToHeight(Sprite* s, float targetH)
{
    if (!s) return;
    const float h = s->getContentSize().height;
    if (h < 0.5f) return;
    s->setScale(targetH / h);
}

CardView* CardView::create(CardModel* cardModel)
{
    CardView* v = new (std::nothrow) CardView();
    if (v && v->initWithModel(cardModel))
    {
        v->autorelease();
        return v;
    }
    CC_SAFE_DELETE(v);
    return nullptr;
}

bool CardView::initWithModel(CardModel* m)
{
    if (!Node::init()) return false;
    _cardModel = m;
    _cardId = m ? m->cardId : -1;

    setContentSize(Size(kCardWidth, kCardHeight));
    setAnchorPoint(Vec2(0.5f, 0.5f));

    const std::string backPath = CardUtils::getCardBackImagePath();
    if (fileExists(backPath))
    {
        auto* sp = Sprite::create(backPath);
        if (sp)
        {
            scaleSpriteToHeight(sp, kCardHeight - 6.0f);
            sp->setPosition(Vec2(kCardWidth * 0.5f, kCardHeight * 0.5f));
            _backPanel = sp;
            addChild(_backPanel, 0);
        }
    }
    if (!_backPanel)
    {
        auto* fallback = LayerColor::create(Color4B(28, 48, 92, 255), (int)kCardWidth - 2, (int)kCardHeight - 2);
        fallback->setPosition(Vec2(1, 1));
        _backPanel = fallback;
        addChild(_backPanel, 0);
    }

    _faceRoot = Node::create();
    _faceRoot->setContentSize(getContentSize());
    addChild(_faceRoot, 1);

    _facePlate = LayerColor::create(Color4B(252, 248, 238, 255), (int)kCardWidth - 4, (int)kCardHeight - 4);
    _facePlate->setIgnoreAnchorPointForPosition(false);
    _facePlate->setAnchorPoint(Vec2(0.5f, 0.5f));
    _facePlate->setPosition(Vec2(kCardWidth * 0.5f, kCardHeight * 0.5f));
    _faceRoot->addChild(_facePlate, 0);

    _faceEdge = DrawNode::create();
    {
        const float w = kCardWidth - 2.0f;
        const float h = kCardHeight - 2.0f;
        const Color4F stroke(0.28f, 0.22f, 0.15f, 0.92f);
        _faceEdge->drawLine(Vec2(1, 1), Vec2(w, 1), stroke);
        _faceEdge->drawLine(Vec2(w, 1), Vec2(w, h), stroke);
        _faceEdge->drawLine(Vec2(w, h), Vec2(1, h), stroke);
        _faceEdge->drawLine(Vec2(1, h), Vec2(1, 1), stroke);
    }
    _faceRoot->addChild(_faceEdge, 1);

    rebuildFaceArt();
    updateDisplay();
    initTouchListener();
    return true;
}

void CardView::rebuildFaceArt()
{
    if (!_cardModel || !_faceRoot) return;

    if (_rankSprite)
    {
        _rankSprite->removeFromParent();
        _rankSprite = nullptr;
    }
    if (_suitCorner)
    {
        _suitCorner->removeFromParent();
        _suitCorner = nullptr;
    }
    if (_suitCenter)
    {
        _suitCenter->removeFromParent();
        _suitCenter = nullptr;
    }
    if (_rankFallback)
    {
        _rankFallback->removeFromParent();
        _rankFallback = nullptr;
    }

    const Size sz = getContentSize();
    const std::string rankPath = CardUtils::getCardImagePath(_cardModel->face, _cardModel->suit, false);

    if (fileExists(rankPath))
    {
        _rankSprite = Sprite::create(rankPath);
        if (_rankSprite)
        {
            _rankSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
            scaleSpriteToHeight(_rankSprite, 38.0f);
            _rankSprite->setPosition(Vec2(26.0f, sz.height - 30.0f));
            _faceRoot->addChild(_rankSprite, 2);
        }
    }
    else
    {
        const bool redSuit = (_cardModel->suit == 1 || _cardModel->suit == 2);
        _rankFallback = Label::createWithSystemFont(CardUtils::getFaceDisplayName(_cardModel->face), "Arial", 34);
        if (_rankFallback)
        {
            _rankFallback->setTextColor(redSuit ? Color4B(200, 32, 32, 255) : Color4B(36, 36, 36, 255));
            _rankFallback->setAnchorPoint(Vec2(0.5f, 0.5f));
            _rankFallback->setPosition(Vec2(26.0f, sz.height - 30.0f));
            _faceRoot->addChild(_rankFallback, 2);
        }
    }

    const std::string suitPath = CardUtils::getSuitImagePath(_cardModel->suit);
    if (fileExists(suitPath))
    {
        _suitCorner = Sprite::create(suitPath);
        if (_suitCorner)
        {
            _suitCorner->setAnchorPoint(Vec2(0.5f, 0.5f));
            scaleSpriteToHeight(_suitCorner, 26.0f);
            _suitCorner->setPosition(Vec2(26.0f, sz.height - 54.0f));
            _faceRoot->addChild(_suitCorner, 3);
        }

        _suitCenter = Sprite::create(suitPath);
        if (_suitCenter)
        {
            _suitCenter->setAnchorPoint(Vec2(0.5f, 0.5f));
            scaleSpriteToHeight(_suitCenter, 72.0f);
            _suitCenter->setOpacity(210);
            _suitCenter->setPosition(Vec2(sz.width * 0.5f, sz.height * 0.5f + 2.0f));
            _faceRoot->addChild(_suitCenter, 4);
        }
    }
}

void CardView::setClickCallback(const std::function<void(int cardId)>& callback)
{
    _clickCallback = callback;
}

void CardView::initTouchListener()
{
    auto* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(CardView::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(CardView::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool CardView::onTouchBegan(Touch* touch, Event* event)
{
    if (!_touchEnabled) return false;
    Node* parent = getParent();
    Vec2 pointInParent = parent ? parent->convertTouchToNodeSpace(touch) : touch->getLocation();
    if (!getBoundingBox().containsPoint(pointInParent))
        return false;
    _touchBeganPosition = touch->getLocation();
    return true;
}

void CardView::onTouchEnded(Touch* touch, Event* event)
{
    Vec2 touchEndPosition = touch->getLocation();
    float distance = touchEndPosition.distance(_touchBeganPosition);
    if (distance < 20.0f)
    {
        if (_clickCallback)
            _clickCallback(_cardId);
    }
}

void CardView::moveToPosition(const Vec2& targetPos, float duration, std::function<void()> callback)
{
    if (duration <= 0)
    {
        setPosition(targetPos);
        if (callback) callback();
        return;
    }
    auto moveTo = MoveTo::create(duration, targetPos);
    if (callback)
    {
        auto seq = Sequence::create(moveTo, CallFunc::create(callback), nullptr);
        runAction(seq);
    }
    else
    {
        runAction(moveTo);
    }
}

void CardView::updateDisplay()
{
    if (!_cardModel) return;
    const bool up = _cardModel->isFaceUp;
    if (_faceRoot)
        _faceRoot->setVisible(up);
    if (_backPanel)
        _backPanel->setVisible(!up);
}

void CardView::setTouchEnabled(bool enabled)
{
    _touchEnabled = enabled;
}

void CardView::syncFromModel()
{
    if (!_cardModel) return;
    updateDisplay();
    bool clickable = true;
    if (_cardModel->pileType == 0)
        clickable = _cardModel->isFaceUp;
    setTouchEnabled(clickable);
}
