/****************************************************************************
 * PlayfieldOcclusionService.cpp
 ****************************************************************************/

#include "PlayfieldOcclusionService.h"
#include "models/CardModel.h"

#include <cmath>

bool PlayfieldOcclusionService::centersOverlap(const cocos2d::Vec2& a, const cocos2d::Vec2& b)
{
    const float hw = kCardHitWidth * 0.52f;
    const float hh = kCardHitHeight * 0.52f;
    return (std::fabs(a.x - b.x) < hw) && (std::fabs(a.y - b.y) < hh);
}

void PlayfieldOcclusionService::refreshExposure(std::vector<CardModel*>& playfield)
{
    const int n = static_cast<int>(playfield.size());
    for (int i = 0; i < n; ++i)
    {
        CardModel* below = playfield[static_cast<size_t>(i)];
        if (!below) continue;

        bool covered = false;
        for (int j = 0; j < n; ++j)
        {
            if (i == j) continue;
            CardModel* above = playfield[static_cast<size_t>(j)];
            if (!above) continue;
            if (above->playfieldDrawOrder <= below->playfieldDrawOrder)
                continue;
            if (centersOverlap(below->position, above->position))
            {
                covered = true;
                break;
            }
        }
        below->isFaceUp = !covered;
    }
}
