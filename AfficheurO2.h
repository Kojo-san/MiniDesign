#pragma once
#include "Afficheur.h"
#include "NuageDePoints.h"
#include "IObservateur.h"

/// Affiche les points en montrant les IDs (0,1,2,...)
class AfficheurO2 : public Afficheur, public IObservateur {
public:
    explicit AfficheurO2(NuageDePoints& n);

    void actualiser() override;

protected:
    void afficherContenu() override;

private:
    NuageDePoints& nuage_;
};
