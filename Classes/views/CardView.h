/****************************************************************************
 * CardView.h - ??????? + ????? + ???? + ???
 ****************************************************************************/

#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include "models/CardModel.h"
#include <functional>

/**
 * ??? number ????????????????????????
 */
class CardView : public cocos2d::Node
{
public:
    static CardView* create(CardModel* cardModel);

    void setClickCallback(const std::function<void(int cardId)>& callback);
    const CardModel* getCardModel() const { return _cardModel; }
    int getCardId() const { return _cardId; }
    void moveToPosition(const cocos2d::Vec2& targetPos, float duration = 0.3f, std::function<void()> callback = nullptr);
    void setTouchEnabled(bool enabled);
    void updateDisplay();
    void syncFromModel();

private:
    bool initWithModel(CardModel* m);
    void initTouchListener();
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void rebuildFaceArt();
    static void scaleSpriteToHeight(cocos2d::Sprite* s, float targetH);

    CardModel* _cardModel = nullptr;
    int _cardId = -1;
    std::function<void(int cardId)> _clickCallback;
    bool _touchEnabled = true;
    cocos2d::Vec2 _touchBeganPosition;

    /** ???Sprite ???? */
    cocos2d::Node* _backPanel = nullptr;
    cocos2d::Node* _faceRoot = nullptr;
    cocos2d::LayerColor* _facePlate = nullptr;
    cocos2d::DrawNode* _faceEdge = nullptr;
    cocos2d::Sprite* _rankSprite = nullptr;
    cocos2d::Sprite* _suitCorner = nullptr;
    cocos2d::Sprite* _suitCenter = nullptr;
    cocos2d::Label* _rankFallback = nullptr;

    static constexpr float kCardWidth = 118.0f;
    static constexpr float kCardHeight = 166.0f;
};

#endif
