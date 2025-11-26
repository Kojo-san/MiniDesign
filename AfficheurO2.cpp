#include "AfficheurO2.h"
#include "Point.h"

AfficheurO2::AfficheurO2(NuageDePoints& n)
    : nuage_(n) {}

void AfficheurO2::actualiser() {
    afficher();
}

void AfficheurO2::afficherContenu() {
    auto elements = nuage_.getElements();

    std::cout << "Affichage IDs des points :\n";
    for (auto e : elements) {
        Point* p = dynamic_cast<Point*>(e);
        if (p) {
            std::cout << "ID " << p->getId()
                      << " -> (" << p->getX() << "," << p->getY() << ")\n";
        }
    }
}
