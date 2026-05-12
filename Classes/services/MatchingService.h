/****************************************************************************
 * MatchingService.h - ¿¨ÅÆÆ¥ÅäÅÐ¶Ï·þÎñ
 ****************************************************************************/

#ifndef __MATCHING_SERVICE_H__
#define __MATCHING_SERVICE_H__

#include "models/CardModel.h"

class MatchingService
{
public:
    static bool isMatch(const CardModel* card1, const CardModel* card2);
    static bool isFaceValueMatch(int faceValue1, int faceValue2);
};

#endif