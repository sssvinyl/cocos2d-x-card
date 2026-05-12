/****************************************************************************
 * HelloWorldScene.cpp
 * Stable start screen and game scene root.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "controllers/GameController.h"
#include "views/PlayFieldView.h"
#include "views/HandPileView.h"
#include "views/StockPileView.h"
#include "views/UndoButtonView.h"

USING_NS_CC;

namespace
{
const float kDividerY = 580.0f;

const char* kTitleText = "\xE7\xBA\xB8\xE7\x89\x8C\xE6\x8E\xA5\xE9\xBE\x99";
const char* kStartText = "\xE5\xBC\x80\xE5\xA7\x8B\xE6\xB8\xB8\xE6\x88\x8F";
const char* kSelectLevelText = "\xE9\x80\x89\xE6\x8B\xA9\xE5\x85\xB3\xE5\x8D\xA1";
const char* kLevelOneText = "\xE7\xAC\xAC 1 \xE5\x85\xB3";
const char* kLevelTwoText = "\xE7\xAC\xAC 2 \xE5\x85\xB3";
const char* kBackText = "\xE8\xBF\x94\xE5\x9B\x9E";
const char* kBackSelectText = "\xE8\xBF\x94\xE5\x9B\x9E\xE9\x80\x89\xE5\x85\xB3";
const char* kSuccessText = "\xE9\x80\x9A\xE5\x85\xB3\xE6\x88\x90\xE5\x8A\x9F";
const char* kContinueText = "\xE7\xBB\xA7\xE7\xBB\xAD\xE6\xB8\xB8\xE6\x88\x8F";
const char* kRuleText = "\xE7\x82\xB9\xE5\x87\xBB\xE7\x82\xB9\xE6\x95\xB0 +1 \xE6\x88\x96 -1 \xE7\x9A\x84\xE6\x98\x8E\xE7\x89\x8C\xE8\xBF\x9B\xE8\xA1\x8C\xE6\xB6\x88\xE9\x99\xA4";
const char* kPlayfieldText = "\xE4\xB8\xBB\xE7\x89\x8C\xE5\x8C\xBA";
const char* kTrayText = "\xE5\xBA\x95\xE7\x89\x8C\xE5\x8C\xBA / \xE5\xA4\x87\xE7\x94\xA8\xE7\x89\x8C / \xE5\x9B\x9E\xE9\x80\x80";

Label* createLabel(const char* text, float size, const Color4B& color)
{
    Label* label = Label::createWithSystemFont(text, "Arial", size);
    if (label)
        label->setTextColor(color);
    return label;
}
}

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

bool HelloWorld::init()
{
    if (!Scene::init()) return false;

    buildBackground();
    showStartScreen();
    return true;
}

void HelloWorld::buildBackground()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    LayerGradient* background = LayerGradient::create(
        Color4B(52, 146, 96, 255),
        Color4B(5, 32, 27, 255),
        Vec2(0.0f, 1.0f));
    background->setContentSize(visibleSize);
    background->setPosition(Vec2::ZERO);
    addChild(background, -20);

    DrawNode* texture = DrawNode::create();
    for (float x = -visibleSize.height; x < visibleSize.width + visibleSize.height; x += 56.0f)
    {
        texture->drawLine(Vec2(x, 0), Vec2(x + visibleSize.height, visibleSize.height), Color4F(1, 1, 1, 0.035f));
        texture->drawLine(Vec2(x + 28.0f, 0), Vec2(x + 28.0f - visibleSize.height, visibleSize.height), Color4F(0, 0, 0, 0.045f));
    }
    addChild(texture, -19);


    DrawNode* divider = DrawNode::create();
    divider->drawSolidRect(Vec2(0, kDividerY - 5.0f), Vec2(visibleSize.width, kDividerY + 5.0f), Color4F(0.03f, 0.14f, 0.10f, 0.75f));
    divider->drawLine(Vec2(0, kDividerY + 6.0f), Vec2(visibleSize.width, kDividerY + 6.0f), Color4F(0.95f, 0.78f, 0.38f, 0.52f));
    addChild(divider, -5);
}

void HelloWorld::showStartScreen()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    _startLayer = LayerColor::create(Color4B(0, 0, 0, 150), (int)visibleSize.width, (int)visibleSize.height);
    addChild(_startLayer, 100);

    LayerColor* panelShadow = LayerColor::create(Color4B(0, 0, 0, 90), 700, 720);
    panelShadow->setIgnoreAnchorPointForPosition(false);
    panelShadow->setAnchorPoint(Vec2(0.5f, 0.5f));
    panelShadow->setPosition(Vec2(visibleSize.width * 0.5f + 10.0f, visibleSize.height * 0.56f - 10.0f));
    _startLayer->addChild(panelShadow, 1);

    LayerColor* panel = LayerColor::create(Color4B(248, 226, 172, 255), 700, 720);
    panel->setIgnoreAnchorPointForPosition(false);
    panel->setAnchorPoint(Vec2(0.5f, 0.5f));
    panel->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.56f));
    _startLayer->addChild(panel, 2);

    Label* title = createLabel(kTitleText, 80.0f, Color4B(86, 46, 18, 255));
    title->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.56f + 210.0f));
    _startLayer->addChild(title, 3);

    Label* rule = createLabel(kRuleText, 30.0f, Color4B(110, 72, 38, 255));
    rule->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.56f + 90.0f));
    _startLayer->addChild(rule, 3);

    Label* startLabel = createLabel(kStartText, 46.0f, Color4B(255, 248, 224, 255));
    MenuItemLabel* startItem = MenuItemLabel::create(startLabel, CC_CALLBACK_1(HelloWorld::onStartButtonClicked, this));
    startItem->setPosition(Vec2::ZERO);

    LayerColor* buttonBg = LayerColor::create(Color4B(38, 132, 80, 255), 360, 100);
    buttonBg->setIgnoreAnchorPointForPosition(false);
    buttonBg->setAnchorPoint(Vec2(0.5f, 0.5f));
    buttonBg->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.56f - 170.0f));
    _startLayer->addChild(buttonBg, 3);

    Menu* menu = Menu::create(startItem, nullptr);
    menu->setPosition(buttonBg->getPosition());
    _startLayer->addChild(menu, 4);

}

void HelloWorld::onStartButtonClicked(Ref*)
{
    showLevelSelectScreen();
}

void HelloWorld::showLevelSelectScreen()
{
    if (_startLayer)
    {
        _startLayer->removeFromParent();
        _startLayer = nullptr;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    _levelSelectLayer = LayerColor::create(Color4B(0, 0, 0, 150), (int)visibleSize.width, (int)visibleSize.height);
    addChild(_levelSelectLayer, 100);

    LayerColor* panelShadow = LayerColor::create(Color4B(0, 0, 0, 90), 760, 820);
    panelShadow->setIgnoreAnchorPointForPosition(false);
    panelShadow->setAnchorPoint(Vec2(0.5f, 0.5f));
    panelShadow->setPosition(Vec2(visibleSize.width * 0.5f + 10.0f, visibleSize.height * 0.56f - 10.0f));
    _levelSelectLayer->addChild(panelShadow, 1);

    LayerColor* panel = LayerColor::create(Color4B(248, 226, 172, 255), 760, 820);
    panel->setIgnoreAnchorPointForPosition(false);
    panel->setAnchorPoint(Vec2(0.5f, 0.5f));
    panel->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.56f));
    _levelSelectLayer->addChild(panel, 2);

    Label* title = createLabel(kSelectLevelText, 70.0f, Color4B(86, 46, 18, 255));
    title->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.56f + 255.0f));
    _levelSelectLayer->addChild(title, 3);

    LayerColor* levelOneBg = LayerColor::create(Color4B(42, 132, 82, 255), 420, 100);
    levelOneBg->setIgnoreAnchorPointForPosition(false);
    levelOneBg->setAnchorPoint(Vec2(0.5f, 0.5f));
    levelOneBg->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.56f + 80.0f));
    _levelSelectLayer->addChild(levelOneBg, 3);

    LayerColor* levelTwoBg = LayerColor::create(Color4B(52, 112, 156, 255), 420, 100);
    levelTwoBg->setIgnoreAnchorPointForPosition(false);
    levelTwoBg->setAnchorPoint(Vec2(0.5f, 0.5f));
    levelTwoBg->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.56f - 55.0f));
    _levelSelectLayer->addChild(levelTwoBg, 3);

    Label* levelOneLabel = createLabel(kLevelOneText, 44.0f, Color4B(255, 248, 224, 255));
    MenuItemLabel* levelOneItem = MenuItemLabel::create(levelOneLabel, CC_CALLBACK_1(HelloWorld::onLevelOneClicked, this));
    levelOneItem->setPosition(levelOneBg->getPosition());

    Label* levelTwoLabel = createLabel(kLevelTwoText, 44.0f, Color4B(255, 248, 224, 255));
    MenuItemLabel* levelTwoItem = MenuItemLabel::create(levelTwoLabel, CC_CALLBACK_1(HelloWorld::onLevelTwoClicked, this));
    levelTwoItem->setPosition(levelTwoBg->getPosition());

    Label* backLabel = createLabel(kBackText, 32.0f, Color4B(96, 62, 32, 255));
    MenuItemLabel* backItem = MenuItemLabel::create(backLabel, CC_CALLBACK_1(HelloWorld::onBackToStartClicked, this));
    backItem->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.56f - 245.0f));

    Menu* menu = Menu::create(levelOneItem, levelTwoItem, backItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    _levelSelectLayer->addChild(menu, 4);

}

void HelloWorld::onLevelOneClicked(Ref*)
{
    startGame(1);
}

void HelloWorld::onLevelTwoClicked(Ref*)
{
    startGame(2);
}

void HelloWorld::onBackToStartClicked(Ref*)
{
    if (_levelSelectLayer)
    {
        _levelSelectLayer->removeFromParent();
        _levelSelectLayer = nullptr;
    }
    showStartScreen();
}

void HelloWorld::startGame(int levelId)
{
    if (_gameStarted) return;
    if (levelId < 1 || levelId > 2) return;

    _gameStarted = true;
    _currentLevelId = levelId;

    if (_levelSelectLayer)
    {
        _levelSelectLayer->removeFromParent();
        _levelSelectLayer = nullptr;
    }

    if (!_levelControllers[levelId])
    {
        _levelControllers[levelId] = GameController::create();
        if (_levelControllers[levelId])
        {
            _levelControllers[levelId]->retain();
            _levelControllers[levelId]->setLevelCompleteCallback([this]() { showSuccessLayer(); });
            _levelControllers[levelId]->startGame(levelId);
        }
    }

    _gameController = _levelControllers[levelId];
    attachCurrentGameViews();
    addGameHud();

    if (_gameController && _gameController->isCompleted())
        showSuccessLayer();
}

void HelloWorld::attachCurrentGameViews()
{
    if (!_gameController) return;

    PlayFieldView* playFieldView = _gameController->getPlayFieldView();
    HandPileView* handPileView = _gameController->getHandPileView();
    StockPileView* stockPileView = _gameController->getStockPileView();
    UndoButtonView* undoButtonView = _gameController->getUndoButtonView();

    if (playFieldView && !playFieldView->getParent()) addChild(playFieldView, 0);
    if (handPileView && !handPileView->getParent()) addChild(handPileView, 0);
    if (stockPileView && !stockPileView->getParent()) addChild(stockPileView, 12);
    if (undoButtonView && !undoButtonView->getParent()) addChild(undoButtonView, 40);
}

void HelloWorld::detachCurrentGameViews()
{
    if (!_gameController) return;

    if (_gameController->getPlayFieldView()) _gameController->getPlayFieldView()->removeFromParent();
    if (_gameController->getHandPileView()) _gameController->getHandPileView()->removeFromParent();
    if (_gameController->getStockPileView()) _gameController->getStockPileView()->removeFromParent();
    if (_gameController->getUndoButtonView()) _gameController->getUndoButtonView()->removeFromParent();

    if (_gameHudRoot)
    {
        _gameHudRoot->removeFromParent();
        _gameHudRoot = nullptr;
    }
    clearSuccessLayer();
}

void HelloWorld::leaveGameToLevelSelect()
{
    detachCurrentGameViews();
    _gameStarted = false;
    _currentLevelId = 0;
    showLevelSelectScreen();
}

void HelloWorld::onGameBackClicked(Ref*)
{
    leaveGameToLevelSelect();
}

void HelloWorld::showSuccessLayer()
{
    if (_successLayer) return;

    Size visibleSize = Director::getInstance()->getVisibleSize();
    _successLayer = Node::create();
    addChild(_successLayer, 80);

    LayerColor* shade = LayerColor::create(Color4B(0, 0, 0, 120), (int)visibleSize.width, (int)visibleSize.height);
    _successLayer->addChild(shade, 0);

    LayerColor* panel = LayerColor::create(Color4B(248, 226, 172, 255), 620, 360);
    panel->setIgnoreAnchorPointForPosition(false);
    panel->setAnchorPoint(Vec2(0.5f, 0.5f));
    panel->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.56f));
    _successLayer->addChild(panel, 1);

    Label* title = createLabel(kSuccessText, 66.0f, Color4B(86, 46, 18, 255));
    title->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.56f + 72.0f));
    _successLayer->addChild(title, 2);

    Label* continueLabel = createLabel(kContinueText, 34.0f, Color4B(44, 132, 82, 255));
    MenuItemLabel* continueItem = MenuItemLabel::create(continueLabel, [this](Ref*) { clearSuccessLayer(); });
    continueItem->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.56f - 82.0f));

    Menu* menu = Menu::create(continueItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    _successLayer->addChild(menu, 3);
}

void HelloWorld::clearSuccessLayer()
{
    if (_successLayer)
    {
        _successLayer->removeFromParent();
        _successLayer = nullptr;
    }
}

void HelloWorld::addGameHud()
{
    if (_gameHudRoot)
    {
        _gameHudRoot->removeFromParent();
        _gameHudRoot = nullptr;
    }

    _gameHudRoot = Node::create();
    addChild(_gameHudRoot, 20);

    Label* playfieldTitle = createLabel(kPlayfieldText, 32.0f, Color4B(238, 220, 168, 230));
    playfieldTitle->setAnchorPoint(Vec2(0.0f, 0.5f));
    playfieldTitle->setPosition(Vec2(36.0f, kDividerY + 42.0f));
    _gameHudRoot->addChild(playfieldTitle, 1);

    Label* trayTitle = createLabel(kTrayText, 28.0f, Color4B(238, 220, 168, 230));
    trayTitle->setAnchorPoint(Vec2(0.0f, 0.5f));
    trayTitle->setPosition(Vec2(36.0f, kDividerY - 42.0f));
    _gameHudRoot->addChild(trayTitle, 1);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Label* backLabel = createLabel(kBackSelectText, 28.0f, Color4B(255, 248, 224, 255));
    MenuItemLabel* backItem = MenuItemLabel::create(backLabel, CC_CALLBACK_1(HelloWorld::onGameBackClicked, this));
    backItem->setPosition(Vec2(visibleSize.width - 120.0f, 2028.0f));
    Menu* menu = Menu::create(backItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    _gameHudRoot->addChild(menu, 2);
}

void HelloWorld::onExit()
{
    for (int i = 1; i <= 2; ++i)
    {
        CC_SAFE_RELEASE_NULL(_levelControllers[i]);
    }
    _gameController = nullptr;
    Scene::onExit();
}
