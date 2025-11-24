#include "observer.h"
#include "domaine.h"
#include "textures.h"
#include <iostream>
#include <vector>

static const int LARGEUR = 30;
static const int HAUTEUR = 20;

// ----- Afficheur -----
void Afficheur::afficher() {
    preparer();
    afficherContenu();
    terminer();
}

void Afficheur::preparer() {
    // rien de particulier ici, mais extensible
}

void Afficheur::terminer() {
    // idem
}

// ----- AfficheurO1 -----
AfficheurO1::AfficheurO1(NuageDePoints& nuage) : nuage_(nuage) {}

void AfficheurO1::actualiser() {
    // On pourrait n'afficher que si nécessaire, ici on redessine tout
    afficher();
}

void AfficheurO1::afficherContenu() {
    std::vector<std::vector<char>> grille(HAUTEUR, std::vector<char>(LARGEUR, ' '));

    for (const auto& elem : nuage_.getElements()) {
        auto p = std::dynamic_pointer_cast<Point>(elem);
        if (!p) continue;
        int x = p->getX();
        int y = p->getY();
        if (x >= 0 && x < LARGEUR && y >= 0 && y < HAUTEUR) {
            std::string t = p->getTexture()->appliquer(*p);
            char c = (t == " " ? '.' : t[0]);   // première lettre
            grille[y][x] = c;
        }
    }

    for (int y = HAUTEUR - 1; y >= 0; --y) {
        for (int x = 0; x < LARGEUR; ++x)
            std::cout << grille[y][x];
        std::cout << '\n';
    }
}

// ----- AfficheurO2 -----
AfficheurO2::AfficheurO2(NuageDePoints& nuage) : nuage_(nuage) {}

void AfficheurO2::actualiser() {
    afficher();
}

void AfficheurO2::afficherContenu() {
    std::cout << "Liste:\n";
    for (const auto& elem : nuage_.getElements()) {
        auto p = std::dynamic_pointer_cast<Point>(elem);
        if (!p) continue;
        std::cout << p->getId() << ": (" << p->getX() << "," << p->getY() << ")\n";
    }
}
