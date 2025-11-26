#pragma once

#include <memory>
#include "IStrategieSurface.h"

/// Point d'entrée unique du patron Stratégie
class GestionnaireSurface {
public:
    void setStrategie(std::shared_ptr<IStrategieSurface> s);
    Surface generer(const NuageDePoints& nuage) const;

private:
    std::shared_ptr<IStrategieSurface> strategie_;
};
