#pragma once
#include "IStrategieSurface.h"

/// Surface selon distance minimale (greedy)
class CreateurSurfaceC2 : public IStrategieSurface {
public:
    Surface creerSurface(const NuageDePoints& nuage) const override;
};
