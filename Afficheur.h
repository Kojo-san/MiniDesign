#pragma once
#include <iostream>

/// Template Method pour afficher une orthèse
class Afficheur {
public:
    virtual ~Afficheur() = default;

    /// Méthode finale
    void afficher() {
        preparer();
        afficherContenu();
        terminer();
    }

protected:
    virtual void preparer() {
        std::cout << "\n--- Début affichage ---\n";
    }

    virtual void afficherContenu() = 0;  // Redéfini par les enfants

    virtual void terminer() {
        std::cout << "--- Fin affichage ---\n\n";
    }
};
