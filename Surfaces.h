#pragma once
#include <vector>
#include "Domaine.h"
#include "affichage.h"

class IStrategieSurface {
    
public:
    virtual ~IStrategieSurface() = default;
    virtual std::vector<Segment> creerSurfaces(const NuageDePoints& nuage) = 0;
};

class CreateurSurfaceC1 : public IStrategieSurface {
public:
    std::vector<Segment> creerSurfaces(const NuageDePoints& nuage) override;
};

class CreateurSurfaceC2 : public IStrategieSurface {
public:
    std::vector<Segment> creerSurfaces(const NuageDePoints& nuage) override;
};

class GestionnaireSurface {
public:
    GestionnaireSurface();

    void setStrategie(std::unique_ptr<IStrategieSurface> s);
    const std::vector<Segment>& generer(const NuageDePoints& nuage);

    const std::vector<Segment>& segmentsCourants() const { return segments_; }

private:
    std::unique_ptr<IStrategieSurface> strategie_;
    std::vector<Segment> segments_;
};
