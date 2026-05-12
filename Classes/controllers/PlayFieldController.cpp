/****************************************************************************
 * PlayFieldController.cpp
 * 桌面牌区控制器实现
 ****************************************************************************/

#include "PlayFieldController.h"
#include "models/GameModel.h"        // 添加
#include "models/CardModel.h"        // 添加
#include "views/PlayFieldView.h"     // 添加
#include "views/CardView.h"          // 添加
#include "controllers/HandPileController.h"  // 添加
#include "services/MatchingService.h"  // 添加
#include "managers/UndoManager.h"    // 添加

USING_NS_CC;

PlayFieldController* PlayFieldController::create(GameModel* gameModel,
    PlayFieldView* playFieldView,
    HandPileController* handPileController,
    UndoManager* undoManager)
{
    PlayFieldController* controller = new (std::nothrow) PlayFieldController();
    if (controller && controller->init(gameModel, playFieldView, handPileController, undoManager))
    {
        controller->autorelease();
        return controller;
    }
    CC_SAFE_DELETE(controller);
    return nullptr;
}

bool PlayFieldController::init(GameModel* gameModel,
    PlayFieldView* playFieldView,
    HandPileController* handPileController,
    UndoManager* undoManager)
{
    if (!gameModel || !playFieldView || !handPileController || !undoManager)
    {
        CCLOG("PlayFieldController: init失败 - 参数为空");
        return false;
    }

    _gameModel = gameModel;
    _playFieldView = playFieldView;
    _handPileController = handPileController;
    _undoManager = undoManager;

    return true;
}

bool PlayFieldController::onPlayFieldCardClicked(int cardId)
{
    CCLOG("PlayFieldController: 桌面卡牌被点击 - cardId:%d", cardId);

    // ========== 需求2：桌面牌与手牌区顶部牌匹配 ==========

    // 1. 获取顶部牌点数
    int topFaceValue = _handPileController->getTopCardFaceValue();
    if (topFaceValue < 0)
    {
        CCLOG("PlayFieldController: 手牌区为空，无法匹配");
        return false;
    }

    // 2. 获取被点击牌的点数
    CardView* clickedCardView = _playFieldView->getCardById(cardId);
    if (!clickedCardView)
    {
        CCLOG("PlayFieldController: 未找到卡牌视图 - cardId:%d", cardId);
        return false;
    }

    const CardModel* clickedCardModel = clickedCardView->getCardModel();
    if (!clickedCardModel)
    {
        CCLOG("PlayFieldController: 卡牌模型为空 - cardId:%d", cardId);
        return false;
    }

    int clickedFaceValue = clickedCardModel->getDisplayValue();

    // 3. 判断是否匹配
    if (!MatchingService::isFaceValueMatch(topFaceValue, clickedFaceValue))
    {
        CCLOG("PlayFieldController: 不匹配 - 顶牌:%d, 点击牌:%d", topFaceValue, clickedFaceValue);
        return false;
    }

    CCLOG("PlayFieldController: 匹配成功！顶牌:%d, 点击牌:%d", topFaceValue, clickedFaceValue);

    // 4. 执行匹配移动
    _handPileController->setCardAsNewTop(clickedCardView, _playFieldView, true);

    // 5. 从主牌区视图移除卡牌
    _playFieldView->removeCard(cardId);

    CCLOG("PlayFieldController: 匹配消除完成 - cardId:%d 移动到顶牌位置", cardId);

    return true;
}

void PlayFieldController::restoreCardToField(CardView* cardView, const cocos2d::Vec2& position)
{
    if (!cardView)
    {
        return;
    }

    // 重新添加到主牌区视图
    _playFieldView->addCard(cardView, position);

    // 平移动画
    cardView->moveToPosition(position, 0.3f);

    CCLOG("PlayFieldController: 恢复卡牌到主牌区 - cardId:%d, pos:(%.0f, %.0f)",
        cardView->getCardId(), position.x, position.y);
}