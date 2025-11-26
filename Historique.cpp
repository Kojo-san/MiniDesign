#include "Historique.h"
#include <iostream>

void Historique::pushUndo(std::shared_ptr<ICommand> cmd) {
    pileUndo_.push(cmd);
    while (!pileRedo_.empty())      // si une nouvelle commande arrive, 
        pileRedo_.pop();            // l'historique redo devient invalide
}

void Historique::undo() {
    if (pileUndo_.empty()) {
        std::cout << "[UNDO] Rien à annuler.\n";
        return;
    }

    auto cmd = pileUndo_.top();
    pileUndo_.pop();

    cmd->annuler();
    pileRedo_.push(cmd);
}

void Historique::redo() {
    if (pileRedo_.empty()) {
        std::cout << "[REDO] Rien à rétablir.\n";
        return;
    }

    auto cmd = pileRedo_.top();
    pileRedo_.pop();

    cmd->executer();
    pileUndo_.push(cmd);
}
