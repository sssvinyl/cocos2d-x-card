/****************************************************************************
 * HelloWorldScene.h
 * Main scene and start screen.
 ****************************************************************************/

#ifndef __HELLO_WORLD_SCENE_H__
#define __HELLO_WORLD_SCENE_H__

#include "cocos2d.h"

class GameController;

/**
 * @class HelloWorld
 * @brief Hosts the welcome screen, game background and root game views.
 */
class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    virtual void onExit() override;
    CREATE_FUNC(HelloWorld);

private:
    void buildBackground();
    void showStartScreen();
    void showLevelSelectScreen();
    void startGame(int levelId);
    void leaveGameToLevelSelect();
    void attachCurrentGameViews();
    void detachCurrentGameViews();
    void showSuccessLayer();
    void clearSuccessLayer();
    void onStartButtonClicked(cocos2d::Ref* sender);
    void onLevelOneClicked(cocos2d::Ref* sender);
    void onLevelTwoClicked(cocos2d::Ref* sender);
    void onBackToStartClicked(cocos2d::Ref* sender);
    void onGameBackClicked(cocos2d::Ref* sender);
    void addGameHud();

    GameController* _gameController = nullptr;
    GameController* _levelControllers[3] = { nullptr, nullptr, nullptr };
    cocos2d::Node* _startLayer = nullptr;
    cocos2d::Node* _levelSelectLayer = nullptr;
    cocos2d::Node* _gameHudRoot = nullptr;
    cocos2d::Node* _successLayer = nullptr;
    int _currentLevelId = 0;
    bool _gameStarted = false;
};

#endif
