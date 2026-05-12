/****************************************************************************
 * HandPileController.h
 * ÊÖÅÆÇø¿ØÖÆÆ÷
 ****************************************************************************/

#ifndef __HAND_PILE_CONTROLLER_H__
#define __HAND_PILE_CONTROLLER_H__

#include "cocos2d.h"
#include "models/UndoModel.h"

class GameModel;
class HandPileView;
class UndoManager;
class CardView;

class HandPileController : public cocos2d::Ref
{
public:
    static HandPileController* create(GameModel* gameModel,
        HandPileView* handPileView,
        UndoManager* undoManager);

    bool init(GameModel* gameModel, HandPileView* handPileView, UndoManager* undoManager);

    bool onHandCardClicked(int cardId);
    int getTopCardId() const;
    int getTopCardFaceValue() const;
    bool setCardAsNewTop(CardView* cardView, cocos2d::Node* fromView, bool recordUndo = true);
    void undoHandSwap(UndoModel* undoModel);
    void undoMatchMove(UndoModel* undoModel);

private:
    GameModel* _gameModel;
    HandPileView* _handPileView;
    UndoManager* _undoManager;
};

#endif // __HAND_PILE_CONTROLLER_H__