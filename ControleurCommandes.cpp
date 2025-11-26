#include "ControleurCommandes.h"
#include <iostream>

ControleurCommandes::ControleurCommandes() {}

void ControleurCommandes::enregistrer(
    const std::string& cle,
    std::function<std::shared_ptr<ICommand>()> createur)
{
    createurs_[cle] = createur;
}

void ControleurCommandes::executerCommande(const std::string& cle) {
    auto it = createurs_.find(cle);

    if (it == createurs_.end()) {
        std::cout << "Commande inconnue : " << cle << "\n";
        return;
    }

    // Fabrique une nouvelle commande selon la clé
    std::shared_ptr<ICommand> cmd = it->second();

    if (!cmd) {
        std::cout << "Erreur : impossible de créer la commande.\n";
        return;
    }

    // Exécute la commande
    cmd->executer();

    // Certaines commandes n'ont pas d'annulation (ex: afficher)
    // On teste si annuler() est une fonction vide ou non.
    // Ici : par design, on considère que toute commande
    // avec annulation *réelle* doit être mise dans undo.
    // Donc CmdAfficherListe, CmdAfficherO1/O2 sont ignorées.
    // CmdFusion, CmdDeplacer, CmdSupprimer → undo OK
    // CmdSurfaceC1/C2 → pas de undo
    try {
        // On tente annuler() dans un contexte factice pour détecter les commandes "vides"
        // Signature : void annuler(), aucune exception n'est lancée
        // Donc on utilise plutôt un flag dans chaque commande
        // Pour rester simple : on push TOUT sauf afficher et surfaces
        if (cle != "a" && cle != "o1" && cle != "o2"
            && cle != "c1" && cle != "c2")
        {
            hist_.pushUndo(cmd);
        }
    }
    catch (...) {
        // Ne rien faire, pas d'annulation
    }
}

void ControleurCommandes::undo() {
    hist_.undo();
}

void ControleurCommandes::redo() {
    hist_.redo();
}
