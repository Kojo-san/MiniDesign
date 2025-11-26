#include "AfficheurO1.h"
#include "Point.h"
#include "ITexture.h"

AfficheurO1::AfficheurO1(NuageDePoints& n)
    : nuage_(n) {}

void AfficheurO1::actualiser() {
    afficher();
}

void AfficheurO1::afficherContenu() {
    auto elements = nuage_.getElements();

    std::cout << "Affichage textures des points :\n";
    for (auto e : elements) {
        Point* p = dynamic_cast<Point*>(e);
        if (p) {
            auto tex = p->getTexture();
            std::string t = tex ? tex->appliquer(*p) : ".";
            std::cout << "Point #" << p->getId()
                      << " (" << p->getX() << "," << p->getY() << ") : "
                      << t << "\n";
        }
    }
}
