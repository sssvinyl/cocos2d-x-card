/****************************************************************************
 * CardModel.h - ��������ʱ����ģ��
 ****************************************************************************/

#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include "cocos2d.h"

class CardModel : public cocos2d::Ref
{
public:
    int cardId;
    int face;
    int suit;
    cocos2d::Vec2 position;
    bool isFaceUp;
    int pileType;
    int pileIndex;
    /** ����������˳��JSON �п�����Ƹ��󣬻������ϲ㲢���ڵ���ǰ���ƣ�-1 ��ʾ�������� */
    int playfieldDrawOrder;

    CardModel()
        : cardId(-1), face(0), suit(0)
        , position(cocos2d::Vec2::ZERO)
        , isFaceUp(false), pileType(0), pileIndex(-1)
        , playfieldDrawOrder(-1)
    {
    }

    static CardModel* create(int id, int f, int s)
    {
        CardModel* model = new (std::nothrow) CardModel();
        if (model)
        {
            model->cardId = id;
            model->face = f;
            model->suit = s;
        }
        return model;
    }

    int getDisplayValue() const
    {
        return face + 1;
    }
};

#endif