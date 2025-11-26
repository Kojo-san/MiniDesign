#pragma once
#include "Afficheur.h"
#include "NuageDePoints.h"
#include "IObservateur.h"

/// Affiche les points avec leurs textures (o, #, mix, etc.)
class AfficheurO1 : public Afficheur, public IObservateur {
public:
    explicit AfficheurO1(NuageDePoints& n);

    void actualiser() override;

protected:
    void afficherContenu() override;

private:
    NuageDePoints& nuage_;
};
