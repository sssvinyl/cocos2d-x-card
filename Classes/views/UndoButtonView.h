/****************************************************************************
 * UndoButtonView.h
 * Undo button view.
 ****************************************************************************/

#ifndef __UNDO_BUTTON_VIEW_H__
#define __UNDO_BUTTON_VIEW_H__

#include "cocos2d.h"
#include <functional>

/**
 * @class UndoButtonView
 * @brief Displays and handles the undo button. It only owns visual state and input callback.
 */
class UndoButtonView : public cocos2d::Node
{
public:
    static UndoButtonView* create();

    /** Sets the callback invoked when the button is clicked. */
    void setClickCallback(const std::function<void()>& callback);

    /** Updates whether the button can be clicked. */
    void setUndoEnabled(bool enabled);

protected:
    virtual bool init() override;

private:
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void refreshVisualState();

    cocos2d::LayerColor* _shadow = nullptr;
    cocos2d::LayerColor* _background = nullptr;
    cocos2d::DrawNode* _border = nullptr;
    cocos2d::Label* _label = nullptr;
    std::function<void()> _clickCallback;
    cocos2d::Vec2 _touchBeganPosition;
    bool _undoEnabled = false;
    bool _touchDown = false;

    static constexpr float kButtonWidth = 160.0f;
    static constexpr float kButtonHeight = 68.0f;
};

#endif
