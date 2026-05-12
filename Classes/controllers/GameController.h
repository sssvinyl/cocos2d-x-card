/****************************************************************************
 * GameController.h - ???????????????????????
 ****************************************************************************/

#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include <functional>

class GameModel;
class PlayFieldView;
class HandPileView;
class StockPileView;
class UndoButtonView;
class UndoManager;
class UndoModel;
class CardModel;

class GameController : public cocos2d::Ref
{
public:
    static GameController* create();
    virtual ~GameController();
    bool init();
    bool startGame(int levelId);

    GameModel* getGameModel() const { return _gameModel; }
    PlayFieldView* getPlayFieldView() const { return _playFieldView; }
    HandPileView* getHandPileView() const { return _handPileView; }
    StockPileView* getStockPileView() const { return _stockPileView; }
    UndoButtonView* getUndoButtonView() const { return _undoButtonView; }
    bool isCompleted() const;
    void setLevelCompleteCallback(const std::function<void()>& callback) { _levelCompleteCallback = callback; }
    void performUndo();

private:
    GameModel* _gameModel = nullptr;
    PlayFieldView* _playFieldView = nullptr;
    HandPileView* _handPileView = nullptr;
    StockPileView* _stockPileView = nullptr;
    UndoButtonView* _undoButtonView = nullptr;
    UndoManager* _undoManager = nullptr;

    bool _isAnimating = false;
    bool _completionNotified = false;
    std::function<void()> _levelCompleteCallback;

    GameModel* loadLevel(int levelId);
    void initViews();
    void applyPlayfieldOcclusionToViews();
    void handleStockPileClicked();
    void handleUndoClicked();
    void handleHandCardClick(int cardId);
    void handlePlayFieldCardClick(int cardId);
    void updateUndoButtonState();
    void unlockInputAfterUndo();
    void notifyLevelCompleteIfNeeded();
    CardModel* findHandCardById(int cardId) const;
    CardModel* findPlayfieldCardById(int cardId) const;
    void undoHandSwap(UndoModel* undoModel);
    void undoMatchMove(UndoModel* undoModel);
    void undoStockDraw(UndoModel* undoModel);
};

#endif
