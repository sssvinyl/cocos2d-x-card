/****************************************************************************
 * UndoManager.cpp
 * ???????????????
 ****************************************************************************/

#include "UndoManager.h"

UndoManager* UndoManager::create()
{
    UndoManager* mgr = new (std::nothrow) UndoManager();
    if (mgr)
    {
        mgr->autorelease();
    }
    return mgr;
}

UndoManager::~UndoManager()
{
    clear();
}

void UndoManager::pushUndo(UndoModel* undoModel)
{
    if (undoModel)
    {
        undoModel->retain();
        _undoStack.push_back(undoModel);
    }
}

UndoModel* UndoManager::popUndo()
{
    if (_undoStack.empty())
    {
        return nullptr;
    }
    UndoModel* model = _undoStack.back();
    _undoStack.pop_back();
    return model;
}

bool UndoManager::hasUndo() const
{
    return !_undoStack.empty();
}

int UndoManager::getUndoCount() const
{
    return (int)_undoStack.size();
}

void UndoManager::clear()
{
    for (UndoModel* model : _undoStack)
    {
        CC_SAFE_RELEASE(model);
    }
    _undoStack.clear();
}