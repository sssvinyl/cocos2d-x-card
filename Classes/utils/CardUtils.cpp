/****************************************************************************
 * CardUtils.cpp
 * Card helper implementation.
 ****************************************************************************/

#include "CardUtils.h"
#include <cstdio>

namespace
{
std::string getFaceFileName(int face)
{
    switch (face)
    {
    case 0: return "A";
    case 1: return "2";
    case 2: return "3";
    case 3: return "4";
    case 4: return "5";
    case 5: return "6";
    case 6: return "7";
    case 7: return "8";
    case 8: return "9";
    case 9: return "10";
    case 10: return "J";
    case 11: return "Q";
    case 12: return "K";
    default: return "A";
    }
}

bool isRedSuit(int suit)
{
    return suit == 1 || suit == 2;
}
}

std::string CardUtils::getSuitSymbol(int suit)
{
    switch (suit)
    {
    case 0: return "C";
    case 1: return "D";
    case 2: return "H";
    case 3: return "S";
    default: return "?";
    }
}

std::string CardUtils::getFaceDisplayName(int face)
{
    switch (face)
    {
    case 0: return "A";
    case 1: return "2";
    case 2: return "3";
    case 3: return "4";
    case 4: return "5";
    case 5: return "6";
    case 6: return "7";
    case 7: return "8";
    case 8: return "9";
    case 9: return "10";
    case 10: return "J";
    case 11: return "Q";
    case 12: return "K";
    default: return "?";
    }
}

std::string CardUtils::getCardImagePath(int face, int suit, bool isBig)
{
    const std::string sizePrefix = isBig ? "big" : "small";
    const std::string colorPrefix = isRedSuit(suit) ? "red" : "black";
    const std::string faceStr = getFaceFileName(face);

    char buf[128] = { 0 };
    std::snprintf(buf, sizeof(buf), "res/res/number/%s_%s_%s.png",
        sizePrefix.c_str(), colorPrefix.c_str(), faceStr.c_str());
    return std::string(buf);
}

std::string CardUtils::getCardBackImagePath()
{
    return "res/res/card_general.png";
}

std::string CardUtils::getSuitImagePath(int suit)
{
    static const char* kNames[] = { "club", "diamond", "heart", "spade" };
    const char* name = (suit >= 0 && suit < 4) ? kNames[suit] : kNames[0];

    char buf[96] = { 0 };
    std::snprintf(buf, sizeof(buf), "res/res/suits/%s.png", name);
    return std::string(buf);
}
