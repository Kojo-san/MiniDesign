#pragma once

#include "Surface.h"
#include <vector>

class NuageDePoints;

/// Interface pour stratégie de génération de surfaces.
class IStrategieSurface {
public:
    virtual ~IStrategieSurface() = default;

    virtual Surface creerSurface(const NuageDePoints& nuage) const = 0;
};
