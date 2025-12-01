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


vector<Segment> CreateurSurfaceC1::creerSurfaces(const NuageDePoints& nuage)
{
    vector<Point*> pts;
    const_cast<NuageDePoints&>(nuage).collecterPoints(pts);
    
    vector<Segment> segs;
    if (pts.size() < 2) return segs;

    // Calculate centroid
    double cx = 0, cy = 0;
    for (auto* p : pts) {
        cx += p->x();
        cy += p->y();
    }
    cx /= pts.size();
    cy /= pts.size();

    // Separate outer points (on convex hull) from inner points
    // For simplicity, consider points at corners as outer (max/min x,y)
    int minX = pts[0]->x(), maxX = pts[0]->x();
    int minY = pts[0]->y(), maxY = pts[0]->y();
    for (auto* p : pts) {
        if (p->x() < minX) minX = p->x();
        if (p->x() > maxX) maxX = p->x();
        if (p->y() < minY) minY = p->y();
        if (p->y() > maxY) minY = p->y();
        if (p->y() > maxY) maxY = p->y();
    }

    // Separate outer (corner) points from inner points
    vector<Point*> outer, inner;
    for (auto* p : pts) {
        bool isCorner = (p->x() == minX || p->x() == maxX) && 
                        (p->y() == minY || p->y() == maxY);
        if (isCorner) {
            outer.push_back(p);
        } else {
            inner.push_back(p);
        }
    }

    // Sort outer points by angle from centroid
    sort(outer.begin(), outer.end(),
        [cx, cy](Point* a, Point* b) {
            double angleA = atan2(a->y() - cy, a->x() - cx);
            double angleB = atan2(b->y() - cy, b->x() - cx);
            return angleA < angleB;
        });

    // Create segments for outer polygon
    for (size_t i = 0; i + 1 < outer.size(); ++i) {
        segs.push_back({ outer[i]->x(), outer[i]->y(),
                         outer[i+1]->x(), outer[i+1]->y() });
    }
    if (!outer.empty()) {
        segs.push_back({ outer.back()->x(), outer.back()->y(),
                         outer.front()->x(), outer.front()->y() });
    }

    // Connect each inner point to nearest outer point
    for (auto* innerPt : inner) {
        Point* nearest = nullptr;
        double minDist = 1e18;
        for (auto* outerPt : outer) {
            double d = dist2(innerPt, outerPt);
            if (d < minDist) {
                minDist = d;
                nearest = outerPt;
            }
        }
        if (nearest) {
            segs.push_back({ innerPt->x(), innerPt->y(),
                             nearest->x(), nearest->y() });
        }
    }

    return segs;
}


vector<Segment> CreateurSurfaceC2::creerSurfaces(const NuageDePoints& nuage)
{
    vector<Point*> pts;
    const_cast<NuageDePoints&>(nuage).collecterPoints(pts);
    vector<Segment> segs;
    if (pts.size() < 2) return segs;

    sort(pts.begin(), pts.end(), [](Point* a, Point* b){ return a->getId() < b->getId(); });

    vector<bool> utilise(pts.size(), false);
    size_t idx = 0;   
    size_t premier = idx;
    utilise[idx] = true;
    size_t utilises = 1;

    while (utilises < pts.size()) {
        size_t meilleur = (size_t)-1;
        double dmin = 1e18;
        for (size_t j = 0; j < pts.size(); ++j) {
            if (utilise[j]) continue;
            double d = dist2(pts[idx], pts[j]);
            if (d < dmin || (d == dmin && pts[j]->getId() < pts[meilleur]->getId())) {
                dmin = d; meilleur = j;
            }
        }
        if (meilleur == (size_t)-1) break;
        segs.push_back({ pts[idx]->x(), pts[idx]->y(),
                         pts[meilleur]->x(), pts[meilleur]->y() });
        idx = meilleur;
        utilise[idx] = true;
        ++utilises;
    }

    segs.push_back({ pts[idx]->x(), pts[idx]->y(),
                     pts[premier]->x(), pts[premier]->y() });

    return segs;
}


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
