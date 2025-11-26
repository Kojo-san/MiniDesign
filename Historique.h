#pragma once
#include <stack>
#include <memory>
#include "ICommand.h"

/// Gestion de l’historique des commandes (undo/redo)
class Historique {
public:
    void pushUndo(std::shared_ptr<ICommand> cmd);

    void undo();
    void redo();

private:
    std::stack<std::shared_ptr<ICommand>> pileUndo_;
    std::stack<std::shared_ptr<ICommand>> pileRedo_;
};
