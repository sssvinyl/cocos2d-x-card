/****************************************************************************
 * PlayFieldView.h - ��������ͼ
 ****************************************************************************/

#ifndef __PLAY_FIELD_VIEW_H__
#define __PLAY_FIELD_VIEW_H__

#include "cocos2d.h"
#include "CardView.h"
#include <functional>
#include <vector>

class PlayFieldView : public cocos2d::Node
{
public:
    static PlayFieldView* create();
    void setCardClickCallback(const std::function<void(int cardId)>& callback);
    void addCard(CardView* cardView, const cocos2d::Vec2& position);
    void removeCard(int cardId);
    void addCardBack(CardView* cardView, const cocos2d::Vec2& position, int zOrder);
    /** ?????????????????????? */
    CardView* takeCard(int cardId);
    CardView* getCardById(int cardId);
    static cocos2d::Size getPlayFieldSize();

protected:
    virtual bool init() override;

private:
    std::vector<CardView*> _cardViews;
    std::function<void(int cardId)> _cardClickCallback;
    static const int kPlayFieldWidth = 1080;
    static const int kPlayFieldHeight = 1500;
};

#endif