#pragma once

#include <map>
#include <string>
#include <functional>
#include <memory>

#include "ICommand.h"
#include "Historique.h"

/// Fabrication et exécution centralisée des commandes.
/// Utilise une map<string, fonction> comme "fabrique de commandes".
class ControleurCommandes {
public:
    ControleurCommandes();

    /// Enregistre une commande : clé → fabrique lambda
    void enregistrer(const std::string& cle,
                     std::function<std::shared_ptr<ICommand>()> createur);

    /// Exécute une commande choisie par l'utilisateur
    void executerCommande(const std::string& cle);

    /// Undo / Redo
    void undo();
    void redo();

private:
    std::map<std::string, std::function<std::shared_ptr<ICommand>()>> createurs_;
    Historique hist_;
};
