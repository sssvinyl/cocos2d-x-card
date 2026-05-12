/****************************************************************************
 * PlayFieldController.h
 * ×ÀÃæÅÆÇø¿ØÖÆÆ÷
 ****************************************************************************/

#ifndef __PLAY_FIELD_CONTROLLER_H__
#define __PLAY_FIELD_CONTROLLER_H__

#include "cocos2d.h"
#include "models/UndoModel.h"

class GameModel;
class PlayFieldView;
class HandPileController;
class UndoManager;
class CardView;

class PlayFieldController : public cocos2d::Ref
{
public:
    static PlayFieldController* create(GameModel* gameModel,
        PlayFieldView* playFieldView,
        HandPileController* handPileController,
        UndoManager* undoManager);

    bool init(GameModel* gameModel,
        PlayFieldView* playFieldView,
        HandPileController* handPileController,
        UndoManager* undoManager);

    bool onPlayFieldCardClicked(int cardId);
    void restoreCardToField(CardView* cardView, const cocos2d::Vec2& position);

private:
    GameModel* _gameModel;
    PlayFieldView* _playFieldView;
    HandPileController* _handPileController;
    UndoManager* _undoManager;
};

#endif // __PLAY_FIELD_CONTROLLER_H__