/****************************************************************************
 * StockPileView.h
 * ??????????????????????????????????
 ****************************************************************************/

#ifndef __STOCK_PILE_VIEW_H__
#define __STOCK_PILE_VIEW_H__

#include "cocos2d.h"
#include <functional>

class StockPileView : public cocos2d::Node
{
public:
    static StockPileView* create();
    void setDrawCallback(const std::function<void()>& cb) { _drawCallback = cb; }
    void setRemainingCount(int count);

protected:
    bool init() override;

private:
    void initTouch();
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void layoutVisuals();

    cocos2d::LayerColor* _backdrop = nullptr;
    cocos2d::Sprite* _backSprite = nullptr;
    cocos2d::Label* _countLabel = nullptr;
    std::function<void()> _drawCallback;
    cocos2d::Vec2 _touchBegan{};
    bool _drawEnabled = true;
};

#endif
