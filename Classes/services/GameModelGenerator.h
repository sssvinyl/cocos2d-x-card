/****************************************************************************
 * GameModelGenerator.h - 游戏模型生成服务
 ****************************************************************************/

#ifndef __GAME_MODEL_GENERATOR_H__
#define __GAME_MODEL_GENERATOR_H__

#include "configs/models/LevelConfig.h"
#include "models/GameModel.h"

class GameModelGenerator
{
public:
    static GameModel* generateGameModel(LevelConfig* config);

private:
    static int _nextCardId;
    static int allocateCardId();
};

#endif