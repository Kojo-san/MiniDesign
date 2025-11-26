#pragma once
#include <vector>
#include "Domaine.h"
#include "affichage.h"

// ======== Patron Stratégie pour les surfaces ========

class IStrategieSurface {
public:
    virtual ~IStrategieSurface() = default;
    virtual std::vector<Segment> creerSurfaces(const NuageDePoints& nuage) = 0;
};

// C1 : ordre croissant des IDs
class CreateurSurfaceC1 : public IStrategieSurface {
public:
    std::vector<Segment> creerSurfaces(const NuageDePoints& nuage) override;
};

// C2 : distance minimale (greedy nearest neighbor)
class CreateurSurfaceC2 : public IStrategieSurface {
public:
    std::vector<Segment> creerSurfaces(const NuageDePoints& nuage) override;
};

// Gestionnaire : choisit la stratégie
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
