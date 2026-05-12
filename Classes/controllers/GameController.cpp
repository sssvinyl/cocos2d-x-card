/****************************************************************************
 * GameController.cpp - ??????????????????????????????????????
 ****************************************************************************/

#include "GameController.h"
#include "models/GameModel.h"
#include "models/CardModel.h"
#include "views/PlayFieldView.h"
#include "views/HandPileView.h"
#include "views/CardView.h"
#include "views/StockPileView.h"
#include "views/UndoButtonView.h"
#include "managers/UndoManager.h"
#include "models/UndoModel.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "services/GameModelGenerator.h"
#include "services/MatchingService.h"
#include "services/PlayfieldOcclusionService.h"
#include <algorithm>

USING_NS_CC;

GameController* GameController::create()
{
    GameController* controller = new (std::nothrow) GameController();
    if (controller && controller->init())
    {
        controller->autorelease();
        return controller;
    }
    CC_SAFE_DELETE(controller);
    return nullptr;
}

GameController::~GameController()
{
    CC_SAFE_RELEASE_NULL(_playFieldView);
    CC_SAFE_RELEASE_NULL(_handPileView);
    CC_SAFE_RELEASE_NULL(_stockPileView);
    CC_SAFE_RELEASE_NULL(_undoButtonView);
    CC_SAFE_RELEASE_NULL(_undoManager);
}

bool GameController::init()
{
    _gameModel = nullptr;
    _playFieldView = nullptr;
    _handPileView = nullptr;
    _stockPileView = nullptr;
    _undoButtonView = nullptr;
    _undoManager = UndoManager::create();
    if (_undoManager) _undoManager->retain();
    _completionNotified = false;
    return true;
}

bool GameController::startGame(int levelId)
{
    _gameModel = loadLevel(levelId);
    if (!_gameModel) return false;

    initViews();

    auto& pf = _gameModel->getPlayfieldCards();
    for (size_t i = 0; i < pf.size(); i++)
    {
        CardView* cv = CardView::create(pf[i]);
        if (!cv) continue;
        cv->setClickCallback([this](int cardId) { handlePlayFieldCardClick(cardId); });
        _playFieldView->addCard(cv, pf[i]->position);
        cv->setLocalZOrder(pf[i]->playfieldDrawOrder);
    }

    auto& hp = _gameModel->getHandPileCards();
    for (size_t i = 0; i < hp.size(); i++)
    {
        CardView* cv = CardView::create(hp[i]);
        if (!cv) continue;
        cv->setClickCallback([this](int cardId) { handleHandCardClick(cardId); });
        _handPileView->addCardToTop(cv);
        cv->syncFromModel();
    }

    if (_stockPileView)
    {
        _stockPileView->setDrawCallback([this]() { handleStockPileClicked(); });
        _stockPileView->setRemainingCount(static_cast<int>(_gameModel->getStockPileCards().size()));
    }

    if (_undoButtonView)
    {
        _undoButtonView->setClickCallback([this]() { handleUndoClicked(); });
        updateUndoButtonState();
    }

    applyPlayfieldOcclusionToViews();

    return true;
}

bool GameController::isCompleted() const
{
    return _gameModel && _gameModel->getPlayfieldCards().empty();
}

void GameController::notifyLevelCompleteIfNeeded()
{
    if (_completionNotified || !isCompleted()) return;
    _completionNotified = true;
    if (_levelCompleteCallback)
        _levelCompleteCallback();
}

GameModel* GameController::loadLevel(int levelId)
{
    char buf[128];
    snprintf(buf, sizeof(buf), "levels/level_%02d.json", levelId);
    LevelConfig* cfg = LevelConfigLoader::loadLevelConfig(buf);
    if (!cfg) return nullptr;
    return GameModelGenerator::generateGameModel(cfg);
}

void GameController::initViews()
{
    if (!_playFieldView)
    {
        _playFieldView = PlayFieldView::create();
        if (_playFieldView) _playFieldView->retain();
    }
    if (!_handPileView)
    {
        _handPileView = HandPileView::create();
        if (_handPileView) _handPileView->retain();
    }
    if (!_stockPileView)
    {
        _stockPileView = StockPileView::create();
        if (_stockPileView) _stockPileView->retain();
    }
    if (!_undoButtonView)
    {
        _undoButtonView = UndoButtonView::create();
        if (_undoButtonView) _undoButtonView->retain();
    }
}

void GameController::applyPlayfieldOcclusionToViews()
{
    if (!_gameModel || !_playFieldView) return;
    auto& playfield = _gameModel->getPlayfieldCards();
    PlayfieldOcclusionService::refreshExposure(playfield);
    for (CardModel* m : playfield)
    {
        if (!m) continue;
        CardView* cv = _playFieldView->getCardById(m->cardId);
        if (!cv) continue;
        cv->syncFromModel();
        cv->setLocalZOrder(m->playfieldDrawOrder);
    }
}

void GameController::handleStockPileClicked()
{
    if (_isAnimating || !_gameModel || !_handPileView || !_stockPileView || !_undoManager) return;

    auto& stock = _gameModel->getStockPileCards();
    auto& hand = _gameModel->getHandPileCards();
    if (stock.empty() || hand.empty()) return;

    CardModel* oldTop = hand.back();
    CardModel* fromStock = stock.back();

    UndoModel* undoModel = UndoModel::create();
    undoModel->opType = UndoModel::OpType::STOCK_DRAW;
    undoModel->movedCardId = fromStock->cardId;
    undoModel->fromPileType = 2;
    undoModel->fromIndex = static_cast<int>(stock.size()) - 1;
    undoModel->fromPosition = fromStock->position;
    undoModel->toPileType = 1;
    undoModel->toIndex = static_cast<int>(hand.size()) - 1;
    undoModel->toPosition = HandPileView::getTopCardPosition();
    undoModel->displacedCardId = oldTop->cardId;
    undoModel->displacedCardNewIndex = static_cast<int>(hand.size()) - 2;
    undoModel->displacedCardModel = oldTop;
    _undoManager->pushUndo(undoModel);

    _handPileView->removeCard(oldTop->cardId);
    hand.pop_back();

    stock.pop_back();
    fromStock->pileType = 1;
    fromStock->playfieldDrawOrder = -1;
    fromStock->isFaceUp = true;
    fromStock->position = HandPileView::getTopCardPosition();
    hand.push_back(fromStock);

    for (size_t i = 0; i < hand.size(); ++i)
        hand[i]->pileIndex = static_cast<int>(i);

    CardView* cv = CardView::create(fromStock);
    if (cv)
    {
        cv->setClickCallback([this](int cid) { handleHandCardClick(cid); });
        _handPileView->addCardToTop(cv);
        cv->syncFromModel();
    }

    _stockPileView->setRemainingCount(static_cast<int>(stock.size()));
    updateUndoButtonState();
}

void GameController::handleUndoClicked()
{
    CCLOG("GameController: handleUndoClicked");
    performUndo();
}

void GameController::handleHandCardClick(int cardId)
{
    if (_isAnimating || !_gameModel || !_handPileView || !_undoManager) return;

    auto& handCards = _gameModel->getHandPileCards();
    if (handCards.empty()) return;

    int clickedIndex = -1;
    for (size_t i = 0; i < handCards.size(); i++)
    {
        if (handCards[i]->cardId == cardId)
        {
            clickedIndex = static_cast<int>(i);
            break;
        }
    }
    if (clickedIndex == -1) return;

    const int topIndex = static_cast<int>(handCards.size()) - 1;
    if (clickedIndex == topIndex) return;

    CardModel* clickedCard = handCards[clickedIndex];
    CardModel* oldTop = handCards.back();
    const Vec2 clickedOldPos = _handPileView->getCardById(clickedCard->cardId)
        ? _handPileView->getCardById(clickedCard->cardId)->getPosition()
        : clickedCard->position;

    UndoModel* undoModel = UndoModel::create();
    undoModel->opType = UndoModel::OpType::HAND_SWAP;
    undoModel->movedCardId = clickedCard->cardId;
    undoModel->fromPileType = 1;
    undoModel->fromIndex = clickedIndex;
    undoModel->fromPosition = clickedOldPos;
    undoModel->toPileType = 1;
    undoModel->toIndex = topIndex;
    undoModel->toPosition = HandPileView::getTopCardPosition();
    undoModel->displacedCardId = oldTop->cardId;
    undoModel->displacedCardNewIndex = clickedIndex;
    _undoManager->pushUndo(undoModel);

    handCards.erase(handCards.begin() + clickedIndex);
    handCards.push_back(clickedCard);

    for (size_t i = 0; i < handCards.size(); ++i)
    {
        handCards[i]->pileIndex = static_cast<int>(i);
        handCards[i]->position = HandPileView::getTopCardPosition();
    }

    CardView* clickedView = _handPileView->getCardById(clickedCard->cardId);
    if (clickedView)
    {
        _isAnimating = true;
        clickedView->setLocalZOrder(static_cast<int>(handCards.size()) + 1);
        clickedView->moveToPosition(HandPileView::getTopCardPosition(), 0.25f, [this, handCards]() {
            _handPileView->reorderCardsToMatch(_gameModel->getHandPileCards());
            _isAnimating = false;
        });
    }
    else
    {
        _handPileView->reorderCardsToMatch(handCards);
    }

    updateUndoButtonState();
}

void GameController::handlePlayFieldCardClick(int cardId)
{
    if (_isAnimating || !_gameModel || !_playFieldView || !_handPileView || !_undoManager) return;

    auto& handCards = _gameModel->getHandPileCards();
    auto& playfieldCards = _gameModel->getPlayfieldCards();
    if (handCards.empty()) return;

    CardModel* topCard = handCards.back();

    CardModel* clickedCard = nullptr;
    int clickedIndex = -1;
    for (size_t i = 0; i < playfieldCards.size(); i++)
    {
        if (playfieldCards[i]->cardId == cardId)
        {
            clickedCard = playfieldCards[i];
            clickedIndex = static_cast<int>(i);
            break;
        }
    }
    if (!clickedCard) return;
    if (!clickedCard->isFaceUp) return;
    if (!MatchingService::isMatch(topCard, clickedCard)) return;

    CardView* cv = _playFieldView->takeCard(cardId);
    if (!cv) return;

    UndoModel* undoModel = UndoModel::create();
    undoModel->opType = UndoModel::OpType::MATCH_MOVE;
    undoModel->movedCardId = clickedCard->cardId;
    undoModel->fromPileType = 0;
    undoModel->fromIndex = clickedIndex;
    undoModel->fromPosition = clickedCard->position;
    undoModel->toPileType = 1;
    undoModel->toIndex = static_cast<int>(handCards.size());
    undoModel->toPosition = HandPileView::getTopCardPosition();
    undoModel->displacedCardId = topCard->cardId;
    undoModel->displacedCardNewIndex = static_cast<int>(handCards.size()) - 1;
    _undoManager->pushUndo(undoModel);

    playfieldCards.erase(playfieldCards.begin() + clickedIndex);
    clickedCard->pileType = 1;
    clickedCard->playfieldDrawOrder = -1;
    clickedCard->isFaceUp = true;
    clickedCard->position = HandPileView::getTopCardPosition();
    handCards.push_back(clickedCard);

    for (size_t i = 0; i < handCards.size(); ++i)
        handCards[i]->pileIndex = static_cast<int>(i);

    cv->setClickCallback([this](int cid) { handleHandCardClick(cid); });
    _handPileView->addCardToTop(cv);
    cv->syncFromModel();
    _isAnimating = true;
    cv->setLocalZOrder(static_cast<int>(handCards.size()) + 1);
    cv->moveToPosition(HandPileView::getTopCardPosition(), 0.25f, [this]() {
        _handPileView->updateCardPositions();
        _isAnimating = false;
    });
    cv->release();

    applyPlayfieldOcclusionToViews();
    notifyLevelCompleteIfNeeded();
    updateUndoButtonState();
}

void GameController::performUndo()
{
    if (!_undoManager || !_undoManager->hasUndo())
    {
        CCLOG("GameController: performUndo ignored, undoCount=%d", _undoManager ? _undoManager->getUndoCount() : -1);
        _isAnimating = false;
        return;
    }
    if (_isAnimating)
    {
        CCLOG("GameController: performUndo forced unlock before undo");
        _isAnimating = false;
    }

    UndoModel* undoModel = _undoManager->popUndo();
    if (!undoModel) return;

    switch (undoModel->opType)
    {
    case UndoModel::OpType::HAND_SWAP:
        undoHandSwap(undoModel);
        break;
    case UndoModel::OpType::MATCH_MOVE:
        undoMatchMove(undoModel);
        break;
    case UndoModel::OpType::STOCK_DRAW:
        undoStockDraw(undoModel);
        break;
    }
    _isAnimating = false;
    undoModel->release();
    if (!isCompleted())
        _completionNotified = false;
    updateUndoButtonState();
}

void GameController::updateUndoButtonState()
{
    if (_undoButtonView)
        _undoButtonView->setUndoEnabled(_undoManager && _undoManager->hasUndo());
}

void GameController::unlockInputAfterUndo()
{
    _isAnimating = false;
    if (_handPileView)
        _handPileView->updateCardPositions();
    applyPlayfieldOcclusionToViews();
    updateUndoButtonState();
}

CardModel* GameController::findHandCardById(int cardId) const
{
    if (!_gameModel) return nullptr;
    for (CardModel* card : _gameModel->getHandPileCards())
        if (card && card->cardId == cardId) return card;
    return nullptr;
}

CardModel* GameController::findPlayfieldCardById(int cardId) const
{
    if (!_gameModel) return nullptr;
    for (CardModel* card : _gameModel->getPlayfieldCards())
        if (card && card->cardId == cardId) return card;
    return nullptr;
}

void GameController::undoHandSwap(UndoModel* undoModel)
{
    if (!undoModel || !_gameModel || !_handPileView) return;

    auto& handCards = _gameModel->getHandPileCards();
    CardModel* movedCard = findHandCardById(undoModel->movedCardId);
    if (!movedCard) return;

    for (auto it = handCards.begin(); it != handCards.end(); ++it)
    {
        if ((*it)->cardId == movedCard->cardId)
        {
            handCards.erase(it);
            break;
        }
    }

    int insertIndex = std::max(0, std::min(undoModel->fromIndex, static_cast<int>(handCards.size())));
    handCards.insert(handCards.begin() + insertIndex, movedCard);
    for (size_t i = 0; i < handCards.size(); ++i)
    {
        handCards[i]->pileIndex = static_cast<int>(i);
        handCards[i]->pileType = 1;
        handCards[i]->position = HandPileView::getTopCardPosition();
    }

    CardView* movedView = _handPileView->getCardById(undoModel->movedCardId);
    _isAnimating = true;
    if (movedView)
    {
        movedView->setLocalZOrder(static_cast<int>(handCards.size()) + 1);
        movedView->moveToPosition(undoModel->fromPosition, 0.25f, [this]() {
            _handPileView->reorderCardsToMatch(_gameModel->getHandPileCards());
            unlockInputAfterUndo();
        });
    }
    else
    {
        _handPileView->reorderCardsToMatch(handCards);
        _isAnimating = false;
    }
}

void GameController::undoMatchMove(UndoModel* undoModel)
{
    if (!undoModel || !_gameModel || !_handPileView || !_playFieldView) return;

    auto& handCards = _gameModel->getHandPileCards();
    auto& playfieldCards = _gameModel->getPlayfieldCards();
    CardModel* movedCard = findHandCardById(undoModel->movedCardId);
    if (!movedCard) return;

    CardView* cv = _handPileView->takeCard(movedCard->cardId);
    if (!cv) return;

    for (auto it = handCards.begin(); it != handCards.end(); ++it)
    {
        if ((*it)->cardId == movedCard->cardId)
        {
            handCards.erase(it);
            break;
        }
    }

    movedCard->pileType = 0;
    movedCard->pileIndex = undoModel->fromIndex;
    movedCard->position = undoModel->fromPosition;
    movedCard->playfieldDrawOrder = undoModel->fromIndex;

    int insertIndex = std::max(0, std::min(undoModel->fromIndex, static_cast<int>(playfieldCards.size())));
    playfieldCards.insert(playfieldCards.begin() + insertIndex, movedCard);

    for (size_t i = 0; i < handCards.size(); ++i)
        handCards[i]->pileIndex = static_cast<int>(i);

    cv->setClickCallback([this](int cid) { handlePlayFieldCardClick(cid); });
    _playFieldView->addCardBack(cv, HandPileView::getTopCardPosition(), movedCard->playfieldDrawOrder);
    cv->syncFromModel();
    _isAnimating = true;
    cv->moveToPosition(undoModel->fromPosition, 0.25f, [this, cv]() {
        applyPlayfieldOcclusionToViews();
        _handPileView->updateCardPositions();
        unlockInputAfterUndo();
        cv->release();
    });
}

void GameController::undoStockDraw(UndoModel* undoModel)
{
    if (!undoModel || !_gameModel || !_handPileView || !_stockPileView) return;

    auto& handCards = _gameModel->getHandPileCards();
    auto& stockCards = _gameModel->getStockPileCards();
    CardModel* drawnCard = findHandCardById(undoModel->movedCardId);
    if (!drawnCard) return;

    _handPileView->removeCard(drawnCard->cardId);
    for (auto it = handCards.begin(); it != handCards.end(); ++it)
    {
        if ((*it)->cardId == drawnCard->cardId)
        {
            handCards.erase(it);
            break;
        }
    }

    drawnCard->pileType = 2;
    drawnCard->pileIndex = static_cast<int>(stockCards.size());
    drawnCard->isFaceUp = false;
    drawnCard->position = undoModel->fromPosition;
    stockCards.push_back(drawnCard);

    CardModel* oldTop = findHandCardById(undoModel->displacedCardId);
    if (!oldTop)
        oldTop = undoModel->displacedCardModel;

    if (oldTop && oldTop->cardId != drawnCard->cardId)
    {
        oldTop->pileType = 1;
        oldTop->isFaceUp = true;
        oldTop->position = HandPileView::getTopCardPosition();
        handCards.push_back(oldTop);
        CardView* restored = CardView::create(oldTop);
        if (restored)
        {
            restored->setClickCallback([this](int cid) { handleHandCardClick(cid); });
            _handPileView->addCardToTop(restored);
            restored->syncFromModel();
        }
    }

    for (size_t i = 0; i < handCards.size(); ++i)
        handCards[i]->pileIndex = static_cast<int>(i);

    _handPileView->updateCardPositions();
    _stockPileView->setRemainingCount(static_cast<int>(stockCards.size()));
}
