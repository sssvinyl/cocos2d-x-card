/****************************************************************************
 * LevelConfig.h - 关卡配置静态数据结构
 ****************************************************************************/

#ifndef __LEVEL_CONFIG_H__
#define __LEVEL_CONFIG_H__

#include "cocos2d.h"
#include <vector>

struct CardConfig
{
    int cardFace;
    int cardSuit;
    cocos2d::Vec2 position;
    CardConfig() : cardFace(0), cardSuit(0), position(cocos2d::Vec2::ZERO) {}
};

class LevelConfig : public cocos2d::Ref
{
public:
    LevelConfig() : _levelId(0) {}

    static LevelConfig* create()
    {
        LevelConfig* config = new (std::nothrow) LevelConfig();
        if (config) config->autorelease();
        return config;
    }

    int getLevelId() const { return _levelId; }
    void setLevelId(int id) { _levelId = id; }
    std::vector<CardConfig>& getPlayfield() { return _playfield; }
    std::vector<CardConfig>& getStack() { return _stack; }
    std::vector<CardConfig>& getStock() { return _stock; }

private:
    int _levelId;
    std::vector<CardConfig> _playfield;
    std::vector<CardConfig> _stack;
    std::vector<CardConfig> _stock;
};

#endif