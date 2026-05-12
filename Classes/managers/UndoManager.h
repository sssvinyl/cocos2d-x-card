/****************************************************************************
 * UndoManager.h
 * 回退功能管理器
 ****************************************************************************/

#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include "models/UndoModel.h"
#include "cocos2d.h"
#include <vector>

class UndoManager : public cocos2d::Ref
{
public:
    static UndoManager* create();
    virtual ~UndoManager();

    void pushUndo(UndoModel* undoModel);
    UndoModel* popUndo();
    bool hasUndo() const;
    int getUndoCount() const;
    void clear();

private:
    std::vector<UndoModel*> _undoStack;
};

#endif // __UNDO_MANAGER_H__