#include "CreateurSurfaceC1.h"
#include "NuageDePoints.h"
#include "Point.h"
#include <algorithm>

Surface CreateurSurfaceC1::creerSurface(const NuageDePoints& nuage) const {
    Surface s;
    auto elems = nuage.getElements();

    std::vector<Point> pts;
    for (auto e : elems) {
        if (auto p = dynamic_cast<Point*>(e))
            pts.push_back(*p);
    }

    std::sort(pts.begin(), pts.end(),
              [](const Point& a, const Point& b){
                  return a.getId() < b.getId();
              });

    s.points = std::move(pts);
    return s;
}
