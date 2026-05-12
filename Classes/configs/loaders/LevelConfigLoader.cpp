/****************************************************************************
 * LevelConfigLoader.cpp - 关卡配置加载器实现
 ****************************************************************************/

#include "LevelConfigLoader.h"
#include "json/document.h"

USING_NS_CC;

LevelConfig* LevelConfigLoader::loadLevelConfig(const std::string& filePath)
{
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filePath);
    std::string jsonContent = FileUtils::getInstance()->getStringFromFile(fullPath);

    if (jsonContent.empty()) return nullptr;

    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(jsonContent.c_str());
    if (doc.HasParseError()) return nullptr;

    LevelConfig* config = LevelConfig::create();
    if (!config) return nullptr;

    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray())
    {
        const rapidjson::Value& arr = doc["Playfield"];
        for (rapidjson::SizeType i = 0; i < arr.Size(); i++)
        {
            CardConfig card;
            const auto& c = arr[i];
            if (c.HasMember("CardFace")) card.cardFace = c["CardFace"].GetInt();
            if (c.HasMember("CardSuit")) card.cardSuit = c["CardSuit"].GetInt();
            if (c.HasMember("Position"))
            {
                if (c["Position"].HasMember("x")) card.position.x = (float)c["Position"]["x"].GetInt();
                if (c["Position"].HasMember("y")) card.position.y = (float)c["Position"]["y"].GetInt();
            }
            config->getPlayfield().push_back(card);
        }
    }

    if (doc.HasMember("Stack") && doc["Stack"].IsArray())
    {
        const rapidjson::Value& arr = doc["Stack"];
        for (rapidjson::SizeType i = 0; i < arr.Size(); i++)
        {
            CardConfig card;
            const auto& c = arr[i];
            if (c.HasMember("CardFace")) card.cardFace = c["CardFace"].GetInt();
            if (c.HasMember("CardSuit")) card.cardSuit = c["CardSuit"].GetInt();
            if (c.HasMember("Position"))
            {
                if (c["Position"].HasMember("x")) card.position.x = (float)c["Position"]["x"].GetInt();
                if (c["Position"].HasMember("y")) card.position.y = (float)c["Position"]["y"].GetInt();
            }
            config->getStack().push_back(card);
        }
    }

    if (doc.HasMember("Stock") && doc["Stock"].IsArray())
    {
        const rapidjson::Value& arr = doc["Stock"];
        for (rapidjson::SizeType i = 0; i < arr.Size(); i++)
        {
            CardConfig card;
            const auto& c = arr[i];
            if (c.HasMember("CardFace")) card.cardFace = c["CardFace"].GetInt();
            if (c.HasMember("CardSuit")) card.cardSuit = c["CardSuit"].GetInt();
            config->getStock().push_back(card);
        }
    }

    return config;
}