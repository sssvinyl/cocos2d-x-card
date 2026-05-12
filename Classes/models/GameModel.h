/****************************************************************************
 * GameModel.h - 游戏全局数据模型
 ****************************************************************************/

#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include "CardModel.h"
#include <vector>

class GameModel : public cocos2d::Ref
{
public:
    GameModel() : _levelId(0), _score(0) {}

    static GameModel* create()
    {
        GameModel* model = new (std::nothrow) GameModel();
        return model;
    }

    std::vector<CardModel*>& getPlayfieldCards() { return _playfieldCards; }
    std::vector<CardModel*>& getHandPileCards() { return _handPileCards; }
    std::vector<CardModel*>& getStockPileCards() { return _stockPileCards; }

    int getLevelId() const { return _levelId; }
    void setLevelId(int id) { _levelId = id; }

    int getScore() const { return _score; }
    void setScore(int s) { _score = s; }

    CardModel* getTopHandCard()
    {
        if (_handPileCards.empty()) return nullptr;
        return _handPileCards.back();
    }

private:
    std::vector<CardModel*> _playfieldCards;
    std::vector<CardModel*> _handPileCards;
    std::vector<CardModel*> _stockPileCards;
    int _levelId;
    int _score;
};

#endif