#pragma once
#include "IStrategieSurface.h"

/// Surface en triant les points par ID croissant.
class CreateurSurfaceC1 : public IStrategieSurface {
public:
    Surface creerSurface(const NuageDePoints& nuage) const override;
};
