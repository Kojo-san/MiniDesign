#include "strategie.h"
#include <algorithm>
#include <cmath>

void GestionnaireSurface::setStrategie(std::shared_ptr<IStrategieSurface> s) {
    strategie_ = std::move(s);
}

Surface GestionnaireSurface::generer(const NuageDePoints& nuage) {
    if (!strategie_)
        return Surface{};
    return strategie_->creerSurface(nuage);
}

// C1 : ordre des IDs
Surface CreateurSurfaceC1::creerSurface(const NuageDePoints& nuage) {
    Surface s;
    std::vector<std::shared_ptr<IElement>> elems = nuage.getElements();
    std::vector<std::shared_ptr<Point>> points;

    for (auto& e : elems) {
        auto p = std::dynamic_pointer_cast<Point>(e);
        if (p) points.push_back(p);
    }
    std::sort(points.begin(), points.end(),
              [](const std::shared_ptr<Point>& a, const std::shared_ptr<Point>& b) {
                  return a->getId() < b->getId();
              });
    for (auto& p : points)
        s.ajouterPoint(*p);
    return s;
}

// C2 : tri par distance minimale (heuristique simple)
static double dist2(const Point& a, const Point& b) {
    double dx = a.getX() - b.getX();
    double dy = a.getY() - b.getY();
    return dx*dx + dy*dy;
}

Surface CreateurSurfaceC2::creerSurface(const NuageDePoints& nuage) {
    Surface s;
    std::vector<std::shared_ptr<IElement>> elems = nuage.getElements();
    std::vector<std::shared_ptr<Point>> points;

    for (auto& e : elems) {
        auto p = std::dynamic_pointer_cast<Point>(e);
        if (p) points.push_back(p);
    }
    if (points.empty())
        return s;

    // heuristique : on part du premier point, et on enchaîne le plus proche
    std::vector<bool> pris(points.size(), false);
    size_t courant = 0;
    for (size_t i = 0; i < points.size(); ++i) {
        pris[courant] = true;
        s.ajouterPoint(*points[courant]);
        double best = 1e30;
        size_t choix = courant;
        for (size_t j = 0; j < points.size(); ++j) {
            if (!pris[j]) {
                double d = dist2(*points[courant], *points[j]);
                if (d < best) {
                    best = d;
                    choix = j;
                }
            }
        }
        courant = choix;
    }
    return s;
}
