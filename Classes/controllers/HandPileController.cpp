/****************************************************************************
 * HandPileController.cpp
 * 手牌区控制器实现
 ****************************************************************************/

#include "HandPileController.h"
#include "models/GameModel.h"
#include "models/CardModel.h"
#include "views/HandPileView.h"
#include "views/CardView.h"
#include "managers/UndoManager.h"

USING_NS_CC;

HandPileController* HandPileController::create(GameModel* gameModel,
    HandPileView* handPileView,
    UndoManager* undoManager)
{
    HandPileController* controller = new (std::nothrow) HandPileController();
    if (controller && controller->init(gameModel, handPileView, undoManager))
    {
        controller->autorelease();
        return controller;
    }
    CC_SAFE_DELETE(controller);
    return nullptr;
}

bool HandPileController::init(GameModel* gameModel,
    HandPileView* handPileView,
    UndoManager* undoManager)
{
    if (!gameModel || !handPileView || !undoManager)
    {
        CCLOG("HandPileController: init失败 - 参数为空");
        return false;
    }

    _gameModel = gameModel;
    _handPileView = handPileView;
    _undoManager = undoManager;

    return true;
}

int HandPileController::getTopCardId() const
{
    const auto& handCards = _gameModel->getHandPileCards();
    if (handCards.empty())
    {
        return -1;
    }
    return handCards.back()->cardId;
}

int HandPileController::getTopCardFaceValue() const
{
    const auto& handCards = _gameModel->getHandPileCards();
    if (handCards.empty())
    {
        return -1;
    }
    return handCards.back()->getDisplayValue();
}

bool HandPileController::onHandCardClicked(int cardId)
{
    CCLOG("HandPileController: 手牌区卡牌被点击 - cardId:%d", cardId);

    auto& handCards = _gameModel->getHandPileCards();

    // 查找被点击卡牌在数组中的索引
    int clickedIndex = -1;
    for (size_t i = 0; i < handCards.size(); i++)
    {
        if (handCards[i]->cardId == cardId)
        {
            clickedIndex = static_cast<int>(i);
            break;
        }
    }

    if (clickedIndex == -1)
    {
        CCLOG("HandPileController: 未找到卡牌 - cardId:%d", cardId);
        return false;
    }

    // 如果已经是顶部牌，不做处理
    int topIndex = static_cast<int>(handCards.size()) - 1;
    if (clickedIndex == topIndex)
    {
        CCLOG("HandPileController: 点击的已经是顶部牌，忽略");
        return false;
    }

    // ========== 需求1：手牌区内部交换 ==========

    // 获取顶部牌信息
    CardModel* topCard = handCards.back();
    int topCardId = topCard->cardId;
    Vec2 topCardPos = HandPileView::getTopCardPosition();

    // 获取被点击牌信息
    CardModel* clickedCard = handCards[clickedIndex];
    Vec2 clickedCardOldPos = clickedCard->position;

    // ===== 记录回退操作 =====
    UndoModel* undoModel = UndoModel::create();
    undoModel->opType = UndoModel::OpType::HAND_SWAP;
    undoModel->movedCardId = clickedCard->cardId;
    undoModel->fromPileType = 1;           // 手牌区
    undoModel->fromIndex = clickedIndex;
    undoModel->fromPosition = clickedCard->position;
    undoModel->toPileType = 1;             // 手牌区
    undoModel->toIndex = topIndex;
    undoModel->toPosition = topCardPos;
    undoModel->displacedCardId = topCardId;
    undoModel->displacedCardNewIndex = clickedIndex;
    _undoManager->pushUndo(undoModel);

    // ===== 更新Model数据 =====
    // 将点击的牌移到数组末尾（成为新的顶部牌）
    handCards.erase(handCards.begin() + clickedIndex);
    handCards.push_back(clickedCard);

    // 更新原顶部牌的索引
    topCard->pileIndex = static_cast<int>(handCards.size()) - 2;

    // 更新被点击牌的索引（现在是顶部）
    clickedCard->pileIndex = static_cast<int>(handCards.size()) - 1;
    clickedCard->position = topCardPos;

    // ===== 执行动画 =====
    CardView* clickedCardView = _handPileView->getCardById(clickedCard->cardId);
    CardView* topCardView = _handPileView->getCardById(topCardId);

    if (clickedCardView)
    {
        // 被点击的牌平移到顶部位置
        clickedCardView->moveToPosition(topCardPos, 0.3f, [this]() {
            _handPileView->updateCardPositions();
            });
    }

    if (topCardView)
    {
        // 原顶部牌移到被点击牌原来的位置
        topCardView->moveToPosition(clickedCardOldPos, 0.3f);
    }

    // 更新显示层次
    _handPileView->updateCardPositions();

    CCLOG("HandPileController: 手牌区内部交换完成 - cardId:%d 成为新顶牌", clickedCard->cardId);

    return true;
}

bool HandPileController::setCardAsNewTop(CardView* cardView, Node* fromView, bool recordUndo)
{
    if (!cardView)
    {
        return false;
    }

    CardModel* cardModel = const_cast<CardModel*>(cardView->getCardModel());
    if (!cardModel)
    {
        return false;
    }

    auto& handCards = _gameModel->getHandPileCards();

    // 获取当前顶部牌信息
    CardModel* oldTopCard = handCards.empty() ? nullptr : handCards.back();
    int oldTopCardId = oldTopCard ? oldTopCard->cardId : -1;
    Vec2 topCardPos = HandPileView::getTopCardPosition();

    // ===== 记录回退操作 =====
    if (recordUndo)
    {
        UndoModel* undoModel = UndoModel::create();
        undoModel->opType = UndoModel::OpType::MATCH_MOVE;
        undoModel->movedCardId = cardModel->cardId;
        undoModel->fromPileType = 0;        // 主牌区
        undoModel->fromIndex = cardModel->pileIndex;
        undoModel->fromPosition = cardModel->position;
        undoModel->toPileType = 1;          // 手牌区
        undoModel->toIndex = static_cast<int>(handCards.size());
        undoModel->toPosition = topCardPos;
        undoModel->displacedCardId = oldTopCardId;
        undoModel->displacedCardNewIndex = static_cast<int>(handCards.size()) - 1;
        _undoManager->pushUndo(undoModel);
    }

    // ===== 更新Model数据 =====
    // 将被点击的牌从主牌区移除
    auto& playfieldCards = _gameModel->getPlayfieldCards();
    for (auto it = playfieldCards.begin(); it != playfieldCards.end(); ++it)
    {
        if ((*it)->cardId == cardModel->cardId)
        {
            playfieldCards.erase(it);
            break;
        }
    }

    // 添加到手牌区顶部
    cardModel->pileType = 1;
    cardModel->pileIndex = static_cast<int>(handCards.size());
    cardModel->position = topCardPos;
    handCards.push_back(cardModel);

    // ===== 执行动画 =====
    // 从来源视图移除
    cardView->retain();
    cardView->removeFromParent();

    // 添加到手牌区视图
    _handPileView->addCardToTop(cardView);
    cardView->release();

    // 平移动画
    cardView->moveToPosition(topCardPos, 0.3f, [this]() {
        _handPileView->updateCardPositions();
        });

    CCLOG("HandPileController: 桌面牌移动到手牌区顶部 - cardId:%d", cardModel->cardId);

    return true;
}

void HandPileController::undoHandSwap(UndoModel* undoModel)
{
    if (!undoModel)
    {
        return;
    }

    auto& handCards = _gameModel->getHandPileCards();

    // 找到被移动的牌（当前是顶部牌）
    CardModel* movedCard = nullptr;
    int movedIndex = -1;
    for (size_t i = 0; i < handCards.size(); i++)
    {
        if (handCards[i]->cardId == undoModel->movedCardId)
        {
            movedCard = handCards[i];
            movedIndex = static_cast<int>(i);
            break;
        }
    }

    if (!movedCard)
    {
        CCLOG("HandPileController: undoHandSwap - 未找到卡牌 cardId:%d", undoModel->movedCardId);
        return;
    }

    // 将被移动的牌从当前位置移回原来的索引位置
    handCards.erase(handCards.begin() + movedIndex);
    handCards.insert(handCards.begin() + undoModel->fromIndex, movedCard);

    // 恢复位置
    movedCard->pileIndex = undoModel->fromIndex;
    movedCard->position = undoModel->fromPosition;

    // 恢复被挤下顶部的牌
    CardModel* displacedCard = nullptr;
    for (size_t i = 0; i < handCards.size(); i++)
    {
        if (handCards[i]->cardId == undoModel->displacedCardId)
        {
            displacedCard = handCards[i];
            displacedCard->pileIndex = static_cast<int>(handCards.size()) - 1;
            displacedCard->position = HandPileView::getTopCardPosition();
            break;
        }
    }

    // ===== 执行回退动画 =====
    CardView* movedCardView = _handPileView->getCardById(undoModel->movedCardId);
    CardView* displacedCardView = _handPileView->getCardById(undoModel->displacedCardId);

    if (movedCardView)
    {
        movedCardView->moveToPosition(undoModel->fromPosition, 0.3f);
    }

    if (displacedCardView)
    {
        displacedCardView->moveToPosition(HandPileView::getTopCardPosition(), 0.3f);
    }

    _handPileView->updateCardPositions();

    CCLOG("HandPileController: undoHandSwap 完成 - cardId:%d 恢复到索引:%d",
        undoModel->movedCardId, undoModel->fromIndex);
}

void HandPileController::undoMatchMove(UndoModel* undoModel)
{
    if (!undoModel)
    {
        return;
    }

    auto& handCards = _gameModel->getHandPileCards();

    // 从手牌区找到被移动的牌
    CardModel* movedCard = nullptr;
    for (auto it = handCards.begin(); it != handCards.end(); ++it)
    {
        if ((*it)->cardId == undoModel->movedCardId)
        {
            movedCard = *it;
            handCards.erase(it);
            break;
        }
    }

    if (!movedCard)
    {
        CCLOG("HandPileController: undoMatchMove - 手牌区未找到卡牌 cardId:%d", undoModel->movedCardId);
        return;
    }

    // 将牌恢复到主牌区
    movedCard->pileType = 0;
    movedCard->pileIndex = undoModel->fromIndex;
    movedCard->position = undoModel->fromPosition;

    auto& playfieldCards = _gameModel->getPlayfieldCards();
    if (undoModel->fromIndex >= 0 && undoModel->fromIndex <= static_cast<int>(playfieldCards.size()))
    {
        playfieldCards.insert(playfieldCards.begin() + undoModel->fromIndex, movedCard);
    }
    else
    {
        playfieldCards.push_back(movedCard);
    }

    // 恢复被替换的顶部牌
    if (undoModel->displacedCardId >= 0)
    {
        CardModel* displacedCard = nullptr;
        for (size_t i = 0; i < handCards.size(); i++)
        {
            if (handCards[i]->cardId == undoModel->displacedCardId)
            {
                displacedCard = handCards[i];
                break;
            }
        }

        if (displacedCard)
        {
            displacedCard->pileIndex = static_cast<int>(handCards.size()) - 1;
            displacedCard->position = HandPileView::getTopCardPosition();
        }
    }

    // ===== 执行回退动画（需要在PlayFieldController中处理） =====
    CardView* movedCardView = _handPileView->getCardById(undoModel->movedCardId);
    if (movedCardView)
    {
        // 动画回调将由PlayFieldController处理，这里只更新模型
    }

    _handPileView->updateCardPositions();

    CCLOG("HandPileController: undoMatchMove 完成 - cardId:%d 恢复到主牌区", undoModel->movedCardId);
}