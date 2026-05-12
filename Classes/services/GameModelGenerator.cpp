/****************************************************************************
 * GameModelGenerator.cpp - ????????????????
 ****************************************************************************/

#include "GameModelGenerator.h"
#include "PlayfieldOcclusionService.h"

int GameModelGenerator::_nextCardId = 0;

int GameModelGenerator::allocateCardId()
{
    return _nextCardId++;
}

GameModel* GameModelGenerator::generateGameModel(LevelConfig* config)
{
    if (!config) return nullptr;

    _nextCardId = 0;

    GameModel* gameModel = GameModel::create();
    if (!gameModel) return nullptr;

    // ??????
    const auto& pf = config->getPlayfield();
    for (size_t i = 0; i < pf.size(); i++)
    {
        CardModel* card = CardModel::create(allocateCardId(), pf[i].cardFace, pf[i].cardSuit);
        if (!card) continue;
        card->position = pf[i].position;
        card->pileType = 0;
        card->pileIndex = (int)i;
        card->playfieldDrawOrder = (int)i;
        gameModel->getPlayfieldCards().push_back(card);
    }

    // ??????
    const auto& st = config->getStack();
    for (size_t i = 0; i < st.size(); i++)
    {
        CardModel* card = CardModel::create(allocateCardId(), st[i].cardFace, st[i].cardSuit);
        if (!card) continue;
        card->position = st[i].position;
        card->isFaceUp = true;
        card->pileType = 1;
        card->pileIndex = (int)i;
        gameModel->getHandPileCards().push_back(card);
    }

    // ???????
    const auto& stock = config->getStock();
    for (size_t i = 0; i < stock.size(); i++)
    {
        CardModel* card = CardModel::create(allocateCardId(), stock[i].cardFace, stock[i].cardSuit);
        if (!card) continue;
        card->position = cocos2d::Vec2(540, 100);
        card->isFaceUp = false;
        card->pileType = 2;
        card->pileIndex = (int)i;
        gameModel->getStockPileCards().push_back(card);
    }
    if (gameModel->getStockPileCards().empty())
    {
        const int demoFaces[] = { 4, 5, 10, 11, 6, 7, 8 };
        const int demoSuits[] = { 1, 2, 0, 3, 0, 1, 2 };
        const int n = (int)(sizeof(demoFaces) / sizeof(demoFaces[0]));
        for (int k = 0; k < n; ++k)
        {
            CardModel* card = CardModel::create(allocateCardId(), demoFaces[k], demoSuits[k]);
            if (!card) continue;
            card->position = cocos2d::Vec2(540, 100);
            card->isFaceUp = false;
            card->pileType = 2;
            card->pileIndex = k;
            gameModel->getStockPileCards().push_back(card);
        }
    }

    PlayfieldOcclusionService::refreshExposure(gameModel->getPlayfieldCards());

    return gameModel;
}