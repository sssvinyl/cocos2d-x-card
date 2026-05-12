/****************************************************************************
 * LevelConfigLoader.h - πÿø®≈‰÷√º”‘ÿ∆˜
 ****************************************************************************/

#ifndef __LEVEL_CONFIG_LOADER_H__
#define __LEVEL_CONFIG_LOADER_H__

#include "configs/models/LevelConfig.h"
#include <string>

class LevelConfigLoader
{
public:
    static LevelConfig* loadLevelConfig(const std::string& filePath);
};

#endif