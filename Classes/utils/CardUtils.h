/****************************************************************************
 * CardUtils.h
 * Card resource path and display-name helpers.
 ****************************************************************************/

#ifndef __CARD_UTILS_H__
#define __CARD_UTILS_H__

#include <string>

/**
 * @class CardUtils
 * @brief Provides independent helper functions for card text and resource paths.
 */
class CardUtils
{
public:
    /** Returns a short suit symbol used by text fallback rendering. */
    static std::string getSuitSymbol(int suit);

    /** Returns the visible card face name, such as A, 2, J, Q, K. */
    static std::string getFaceDisplayName(int face);

    /** Returns the number image resource path. */
    static std::string getCardImagePath(int face, int suit, bool isBig = true);

    /** Returns the shared card back image resource path. */
    static std::string getCardBackImagePath();

    /** Returns the suit image resource path. */
    static std::string getSuitImagePath(int suit);
};

#endif
