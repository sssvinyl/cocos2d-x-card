/****************************************************************************
 * HandPileView.h - ��������?
 ****************************************************************************/

#ifndef __HAND_PILE_VIEW_H__
#define __HAND_PILE_VIEW_H__

#include "cocos2d.h"
#include "CardView.h"
#include <functional>
#include <vector>

class CardModel;

class HandPileView : public cocos2d::Node
{
public:
    static HandPileView* create();
    void setCardClickCallback(const std::function<void(int cardId)>& callback);
    void addCardToTop(CardView* cardView);
    void removeCard(int cardId);
    CardView* takeCard(int cardId);
    CardView* getCardById(int cardId);
    CardView* getTopCard();
    /** ???????? 0 ??????????????? */
    void reorderCardsToMatch(const std::vector<CardModel*>& handModels);
    void updateCardPositions();
    static cocos2d::Vec2 getTopCardPosition();
    static cocos2d::Size getHandPileSize();

protected:
    virtual bool init() override;

private:
    std::vector<CardView*> _cardViews;
    std::function<void(int cardId)> _cardClickCallback;
    static const int kHandPileWidth = 1080;
    static const int kHandPileHeight = 580;
    static const int kCardStackOffsetY = 78;
};

#endif