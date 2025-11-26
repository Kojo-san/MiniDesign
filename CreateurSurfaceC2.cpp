#include "CreateurSurfaceC2.h"
#include "NuageDePoints.h"
#include "Point.h"
#include <cmath>
#include <vector>
#include <limits>

static double dist(const Point& a, const Point& b) {
    double dx = a.getX() - b.getX();
    double dy = a.getY() - b.getY();
    return std::sqrt(dx*dx + dy*dy);
}

Surface CreateurSurfaceC2::creerSurface(const NuageDePoints& nuage) const {
    Surface s;
    auto elems = nuage.getElements();

    std::vector<Point> pts;
    for (auto e : elems) {
        if (auto p = dynamic_cast<Point*>(e))
            pts.push_back(*p);
    }

    if (pts.empty())
        return s;

    std::vector<bool> pris(pts.size(), false);
    int courant = 0;
    pris[0] = true;
    s.points.push_back(pts[0]);

    for (size_t k = 1; k < pts.size(); ++k) {
        int nextBest = -1;
        double bestDist = std::numeric_limits<double>::max();

        for (size_t i = 0; i < pts.size(); ++i) {
            if (!pris[i]) {
                double d = dist(pts[courant], pts[i]);
                if (d < bestDist) {
                    bestDist = d;
                    nextBest = static_cast<int>(i);
                }
            }
        }
        pris[nextBest] = true;
        courant = nextBest;
        s.points.push_back(pts[nextBest]);
    }

    return s;
}
