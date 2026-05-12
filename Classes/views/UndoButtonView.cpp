/****************************************************************************
 * UndoButtonView.cpp
 * Undo button view implementation.
 ****************************************************************************/

#include "UndoButtonView.h"

USING_NS_CC;

UndoButtonView* UndoButtonView::create()
{
    UndoButtonView* view = new (std::nothrow) UndoButtonView();
    if (view && view->init())
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool UndoButtonView::init()
{
    if (!Node::init()) return false;

    setContentSize(Size(kButtonWidth, kButtonHeight));
    setAnchorPoint(Vec2(0.5f, 0.5f));
    setIgnoreAnchorPointForPosition(false);
    setPosition(Vec2(110.0f, 300.0f));

    _shadow = LayerColor::create(Color4B(0, 0, 0, 80), (int)kButtonWidth, (int)kButtonHeight);
    _shadow->setPosition(Vec2(4.0f, -4.0f));
    addChild(_shadow, 0);

    _background = LayerColor::create(Color4B(248, 220, 150, 255), (int)kButtonWidth, (int)kButtonHeight);
    _background->setPosition(Vec2::ZERO);
    addChild(_background, 1);

    _border = DrawNode::create();
    addChild(_border, 2);

    _label = Label::createWithSystemFont("\xE5\x9B\x9E\xE9\x80\x80", "Arial", 28);
    _label->setPosition(Vec2(kButtonWidth * 0.5f, kButtonHeight * 0.5f));
    addChild(_label, 3);

    auto* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(UndoButtonView::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(UndoButtonView::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithFixedPriority(listener, -128);

    setUndoEnabled(false);
    return true;
}

void UndoButtonView::setClickCallback(const std::function<void()>& callback)
{
    _clickCallback = callback;
}

void UndoButtonView::setUndoEnabled(bool enabled)
{
    _undoEnabled = enabled;
    refreshVisualState();
}

bool UndoButtonView::onTouchBegan(Touch* touch, Event*)
{
    if (!_undoEnabled || !_clickCallback) return false;

    Vec2 pointInNode = convertToNodeSpace(touch->getLocation());
    Rect hitRect(0.0f, 0.0f, kButtonWidth, kButtonHeight);
    if (!hitRect.containsPoint(pointInNode)) return false;

    _touchBeganPosition = touch->getLocation();
    _touchDown = true;
    refreshVisualState();
    return true;
}

void UndoButtonView::onTouchEnded(Touch* touch, Event*)
{
    if (!_touchDown) return;

    _touchDown = false;
    refreshVisualState();

    Vec2 pointInNode = convertToNodeSpace(touch->getLocation());
    Rect hitRect(0.0f, 0.0f, kButtonWidth, kButtonHeight);
    if (!hitRect.containsPoint(pointInNode)) return;
    if (touch->getLocation().distance(_touchBeganPosition) > 24.0f) return;

    CCLOG("UndoButtonView: clicked");
    if (_clickCallback)
        _clickCallback();
}

void UndoButtonView::refreshVisualState()
{
    const bool active = _undoEnabled && !_touchDown;
    const Color4B bgColor = active
        ? Color4B(248, 220, 150, 255)
        : (_undoEnabled ? Color4B(220, 185, 112, 255) : Color4B(118, 108, 94, 180));
    const Color4B borderColor = _undoEnabled
        ? Color4B(106, 68, 30, 255)
        : Color4B(78, 72, 66, 170);

    if (_background)
        _background->setColor(Color3B(bgColor.r, bgColor.g, bgColor.b));
    if (_background)
        _background->setOpacity(bgColor.a);
    if (_label)
        _label->setTextColor(_undoEnabled ? Color4B(76, 46, 20, 255) : Color4B(170, 160, 145, 255));
    if (_shadow)
        _shadow->setVisible(_undoEnabled);

    if (_border)
    {
        _border->clear();
        const Color4F c(borderColor.r / 255.0f, borderColor.g / 255.0f, borderColor.b / 255.0f, borderColor.a / 255.0f);
        _border->drawLine(Vec2(0, 0), Vec2(kButtonWidth, 0), c);
        _border->drawLine(Vec2(kButtonWidth, 0), Vec2(kButtonWidth, kButtonHeight), c);
        _border->drawLine(Vec2(kButtonWidth, kButtonHeight), Vec2(0, kButtonHeight), c);
        _border->drawLine(Vec2(0, kButtonHeight), Vec2(0, 0), c);
    }
}
