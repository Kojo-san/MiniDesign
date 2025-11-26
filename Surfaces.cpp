#include "Surfaces.h"
#include <algorithm>
#include <cmath>

using namespace std;

static double dist2(const Point* a, const Point* b)
{
    double dx = double(a->x() - b->x());
    double dy = double(a->y() - b->y());
    return dx*dx + dy*dy;
}

// ==== C1 : ordre d’ID ====

vector<Segment> CreateurSurfaceC1::creerSurfaces(const NuageDePoints& nuage)
{
    vector<Point*> pts;
    const_cast<NuageDePoints&>(nuage).collecterPoints(pts);
    sort(pts.begin(), pts.end(),
        [](Point* a, Point* b){ return a->getId() < b->getId(); });

    vector<Segment> segs;
    if (pts.size() < 2) return segs;

    for (size_t i = 0; i + 1 < pts.size(); ++i) {
        segs.push_back({ pts[i]->x(), pts[i]->y(),
                         pts[i+1]->x(), pts[i+1]->y() });
    }
    // ferme la forme
    segs.push_back({ pts.back()->x(), pts.back()->y(),
                     pts.front()->x(), pts.front()->y() });

    return segs;
}

// ==== C2 : nearest-neighbor ====

vector<Segment> CreateurSurfaceC2::creerSurfaces(const NuageDePoints& nuage)
{
    vector<Point*> pts;
    const_cast<NuageDePoints&>(nuage).collecterPoints(pts);
    vector<Segment> segs;
    if (pts.size() < 2) return segs;

    vector<bool> utilise(pts.size(), false);
    size_t idx = 0;   // commence au point 0
    size_t premier = idx;
    utilise[idx] = true;
    size_t utilises = 1;

    while (utilises < pts.size()) {
        size_t meilleur = -1;
        double dmin = 1e18;
        for (size_t j = 0; j < pts.size(); ++j) {
            if (utilise[j]) continue;
            double d = dist2(pts[idx], pts[j]);
            if (d < dmin) { dmin = d; meilleur = j; }
        }
        if (meilleur == (size_t)-1) break;
        segs.push_back({ pts[idx]->x(), pts[idx]->y(),
                         pts[meilleur]->x(), pts[meilleur]->y() });
        idx = meilleur;
        utilise[idx] = true;
        ++utilises;
    }

    // relier le dernier au premier
    segs.push_back({ pts[idx]->x(), pts[idx]->y(),
                     pts[premier]->x(), pts[premier]->y() });

    return segs;
}

// ==== GestionnaireSurface ====

GestionnaireSurface::GestionnaireSurface()
    : strategie_(make_unique<CreateurSurfaceC1>()) {}

void GestionnaireSurface::setStrategie(unique_ptr<IStrategieSurface> s)
{
    strategie_ = std::move(s);
}

const vector<Segment>& GestionnaireSurface::generer(const NuageDePoints& nuage)
{
    if (strategie_)
        segments_ = strategie_->creerSurfaces(nuage);
    else
        segments_.clear();
    return segments_;
}
