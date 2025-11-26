#include "GestionnaireSurface.h"
#include "CreateurSurfaceC1.h"
#include "CreateurSurfaceC2.h"
#include "NuageDePoints.h"

void GestionnaireSurface::setStrategie(std::shared_ptr<IStrategieSurface> s) {
    strategie_ = std::move(s);
}

Surface GestionnaireSurface::generer(const NuageDePoints& nuage) const {
    if (!strategie_)
        return {};
    return strategie_->creerSurface(nuage);
}
