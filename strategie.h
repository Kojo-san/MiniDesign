#pragma once
#include "domaine.h"

class IStrategieSurface {
public:
    virtual ~IStrategieSurface() = default;
    virtual Surface creerSurface(const NuageDePoints& nuage) = 0;
};

class CreateurSurfaceC1 : public IStrategieSurface {
public:
    Surface creerSurface(const NuageDePoints& nuage) override;
};

class CreateurSurfaceC2 : public IStrategieSurface {
public:
    Surface creerSurface(const NuageDePoints& nuage) override;
};

class GestionnaireSurface {
public:
    void setStrategie(std::shared_ptr<IStrategieSurface> s);
    Surface generer(const NuageDePoints& nuage);
private:
    std::shared_ptr<IStrategieSurface> strategie_;
};
