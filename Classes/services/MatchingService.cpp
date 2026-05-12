/****************************************************************************
 * MatchingService.cpp - 卡牌匹配判断服务实现
 ****************************************************************************/

#include "MatchingService.h"

bool MatchingService::isMatch(const CardModel* card1, const CardModel* card2)
{
    if (!card1 || !card2) return false;
    return isFaceValueMatch(card1->getDisplayValue(), card2->getDisplayValue());
}

bool MatchingService::isFaceValueMatch(int faceValue1, int faceValue2)
{
    int diff = faceValue1 - faceValue2;
    if (diff < 0) diff = -diff;
    return (diff == 1);
}